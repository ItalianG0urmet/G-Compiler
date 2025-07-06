#include "../include/llvm.h"

#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>
#include <stdlib.h>

static void generateFunctionIR(Function* fun, LLVMContextRef* context,
                               LLVMModuleRef* module) {
    // Default var
    const LLVMTypeRef voidType = LLVMVoidType();
    const LLVMTypeRef intType = LLVMInt32Type();
    const LLVMTypeRef charType = LLVMInt8Type();
    const LLVMTypeRef floatType = LLVMFloatType();

    // Find function return type
    LLVMTypeRef returnType;
    switch (fun->returnType) {
        case RET_VOID:
            returnType = voidType;
            break;
        case RET_INT:
            returnType = intType;
            break;
        case RET_CHAR:
            returnType = charType;
            break;
        case RET_FLOAT:
            returnType = floatType;
            break;
        default:
            fprintf(stderr, "[-] Can't find return type in IR \n");
    }

    LLVMTypeRef paramTypes[] = {};  // TODO: I should make this variable
    LLVMTypeRef funcType = LLVMFunctionType(intType, paramTypes, 0, 0);
    LLVMValueRef func = LLVMAddFunction(*module, fun->name, funcType);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(func, "entry");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMPositionBuilderAtEnd(builder, entry);

    Node* node = fun->body->body;
    while (node != NULL) {
        switch (node->type) {
            // TODO: case NO_ASSIGN_INT_NDEF:
            case NO_ASSIGN_INT: {
                LLVMValueRef var =
                    LLVMBuildAlloca(builder, intType, node->name);
                LLVMBuildStore(builder, LLVMConstInt(intType, node->number, 0),
                               var);
                break;
            }

            case NO_ASSIGN_CHAR: {
                LLVMValueRef var =
                    LLVMBuildAlloca(builder, charType, node->name);
                LLVMBuildStore(builder,
                               LLVMConstInt(charType, node->letter, 0), var);
                break;
            }
            case NO_PRINT: {
                LLVMValueRef str =
                    LLVMBuildGlobalStringPtr(builder, node->name, "str_const");
                LLVMTypeRef charPtrType = LLVMPointerType(LLVMInt8Type(), 0);
                LLVMTypeRef putsType =
                    LLVMFunctionType(intType, &charPtrType, 1, 0);
                LLVMValueRef putsFunc = LLVMGetNamedFunction(*module, "puts");
                if (!putsFunc) {
                    putsFunc = LLVMAddFunction(*module, "puts", putsType);
                }
                LLVMValueRef args[] = {str};
                LLVMBuildCall2(builder, putsType, putsFunc, args, 1, "");
                break;
            }

            case NO_RETURN: {
                if (fun->returnType == RET_VOID) {
                    LLVMBuildRetVoid(builder);
                } else {
                    LLVMBuildRet(builder,
                                 LLVMConstInt(intType, node->number, 0));
                }
                break;
            }

            default: {
                printf("[P] Can't generate the IR for the function %s\n",
                       fun->name);
                exit(1);
                break;
            }
        }
        node = node->next;
    }
}

void generateLLVM(FunctionList functionList) {
    // Find main function
    LLVMContextRef context = LLVMContextCreate();
    LLVMModuleRef module = LLVMModuleCreateWithName("module");
    Function* mainFun = getFunctionByName("main", functionList);
    if (mainFun == NULL) {
        fprintf(stderr, "[-] Main entry point don't exist \n");
        exit(1);
    }

    // Print for debug
    printf("------ IR Generation -------\n");
    generateFunctionIR(mainFun, &context, &module);
    char* ir = LLVMPrintModuleToString(module);
    printf("Generated IR:\n%s\n", ir);
    printf("----------------------------\n");
    LLVMDisposeMessage(ir);

    // Module validation
    char* verify_error = NULL;
    if (LLVMVerifyModule(module, LLVMAbortProcessAction, &verify_error)) {
        fprintf(stderr, "[-] Module verification failed: %s\n", verify_error);
        LLVMDisposeMessage(verify_error);
        exit(1);
    }

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();

    // Configure the target machine
    char* triple = LLVMGetDefaultTargetTriple();
    LLVMTargetRef target;
    char* error = NULL;

    if (LLVMGetTargetFromTriple(triple, &target, &error)) {
        fprintf(stderr, "[-] Failed to get target: %s\n", error);
        LLVMDisposeMessage(error);
        LLVMDisposeMessage(triple);
        exit(1);
    }

    LLVMTargetMachineRef target_machine =
        LLVMCreateTargetMachine(target, triple, "", "", LLVMCodeGenLevelDefault,
                                LLVMRelocDefault, LLVMCodeModelDefault);
    LLVMDisposeMessage(triple);

    // Create object file
    if (LLVMTargetMachineEmitToFile(target_machine, module, "output.o",
                                    LLVMObjectFile, &error)) {
        fprintf(stderr, "[-] Failed to emit object file: %s\n", error);
        LLVMDisposeMessage(error);
        exit(1);
    }

    // Link the object file
    printf("[+] Linking to create executable...\n");
    int linkResult = system("clang output.o -o output");
    if (linkResult != 0) {
        fprintf(stderr, "[-] Linking failed\n");
        exit(1);
    }

    printf("[+] Executable 'output' created successfully!\n");

    // Cleanup
    LLVMDisposeModule(module);
    LLVMContextDispose(context);
}

