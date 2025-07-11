#include "../include/llvm.h"

#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>
#include <stdlib.h>

static void generate_function_ir(function_t* fun, LLVMContextRef* context,
                                 const LLVMModuleRef* module) {
    // Default types
    const LLVMTypeRef void_type = LLVMVoidType();
    const LLVMTypeRef int_type = LLVMInt32Type();
    const LLVMTypeRef char_type = LLVMInt8Type();
    const LLVMTypeRef float_type = LLVMFloatType();

    // Find function return type
    LLVMTypeRef return_type;
    switch (fun->return_type) {
        case RET_VOID:
            return_type = void_type;
            break;
        case RET_INT:
            return_type = int_type;
            break;
        case RET_CHAR:
            return_type = char_type;
            break;
        case RET_FLOAT:
            return_type = float_type;
            break;
        default:
            fprintf(stderr, "[-] Unknown return type for function %s\n",
                    fun->name);
            exit(1);
    }

    const LLVMTypeRef funcType =
        LLVMFunctionType(return_type, NULL, 0, 0);  // TODO: Add more args
    const LLVMValueRef func = LLVMAddFunction(*module, fun->name, funcType);
    const LLVMBasicBlockRef entry = LLVMAppendBasicBlock(func, "entry");
    const LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMPositionBuilderAtEnd(builder, entry);

    const node_t* node = fun->body->body;
    while (node != NULL) {
        switch (node->type) {
            // TODO: case NO_ASSIGN_INT_NDEF:
            case NO_ASSIGN_INT: {
                const LLVMValueRef var =
                    LLVMBuildAlloca(builder, int_type, node->name);
                LLVMBuildStore(builder, LLVMConstInt(int_type, node->number, 0),
                               var);
                break;
            }

            case NO_ASSIGN_FLOAT: {
                const LLVMValueRef var =
                    LLVMBuildAlloca(builder, float_type, node->name);
                LLVMBuildStore(builder,
                               LLVMConstReal(float_type, node->floating), var);

                break;
            }

            case NO_ASSIGN_CHAR: {
                const LLVMValueRef var =
                    LLVMBuildAlloca(builder, char_type, node->name);
                LLVMBuildStore(builder,
                               LLVMConstInt(char_type, node->letter, 0), var);
                break;
            }
            case NO_PRINT: {
                const LLVMValueRef str =
                    LLVMBuildGlobalStringPtr(builder, node->name, "str_const");
                LLVMTypeRef charPtrType = LLVMPointerType(LLVMInt8Type(), 0);
                const LLVMTypeRef putsType =
                    LLVMFunctionType(int_type, &charPtrType, 1, 0);
                LLVMValueRef putsFunc = LLVMGetNamedFunction(*module, "puts");
                if (!putsFunc) {
                    putsFunc = LLVMAddFunction(*module, "puts", putsType);
                }
                LLVMValueRef args[] = {str};
                LLVMBuildCall2(builder, putsType, putsFunc, args, 1, "");
                break;
            }

            case NO_RETURN: {
                switch (fun->return_type) {
                    case RET_VOID:
                        LLVMBuildRetVoid(builder);
                        break;
                    case RET_INT:
                        LLVMBuildRet(builder,
                                     LLVMConstInt(int_type, node->number, 0));
                        break;
                    case RET_FLOAT:
                        LLVMBuildRet(builder,
                                     LLVMConstReal(float_type, node->floating));
                        break;
                    case RET_CHAR:
                        LLVMBuildRet(builder,
                                     LLVMConstInt(char_type, node->letter, 0));
                        break;
                    default:
                        fprintf(stderr, "[-] Invalid return in %s\n",
                                fun->name);
                        exit(1);
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

void generate_llvm(const function_list_t function_list) {
    // Find main function
    LLVMContextRef context = LLVMContextCreate();
    const LLVMModuleRef module = LLVMModuleCreateWithName("module");
    function_t* main_fun = get_function_by_name("main", function_list);
    if (main_fun == NULL) {
        fprintf(stderr, "[-] Main entry point don't exist \n");
        exit(1);
    }

    // Print for debug
    printf("------ IR Generation -------\n");
    generate_function_ir(main_fun, &context, &module);
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

    const LLVMTargetMachineRef target_machine =
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
    const int link_result = system("clang output.o -o output");
    if (link_result != 0) {
        fprintf(stderr, "[-] Linking failed\n");
        exit(1);
    }

    printf("[+] Executable 'output' created successfully!\n");

    // Cleanup
    LLVMDisposeModule(module);
    LLVMContextDispose(context);
}

