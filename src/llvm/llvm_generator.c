#include "llvm_generator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "llvm_return.h"
#include "llvm_types.h"
#include "llvm_var.h"

static LLVMTypeRef find_return_type(struct Function* fun,
                                    const struct LLVMTypeTable* table) {
    switch (fun->return_type) {
        case RET_VOID:
            return table->void_type;
            break;
        case RET_INT:
            return table->int_type;
            break;
        case RET_CHAR:
            return table->char_type;
            break;
        case RET_FLOAT:
            return table->float_type;
            break;
        default:
            fprintf(stderr, "[-] Unknown return type for function %s\n",
                    fun->name);
            exit(1);
    }
}

void llvm_generator_generate_function_ir(struct Function* fun,
                                         LLVMContextRef* context,
                                         const LLVMModuleRef* module) {
    // Default types
    const struct LLVMTypeTable types_table = {LLVMVoidType(), LLVMInt32Type(),
                                              LLVMFloatType(), LLVMInt8Type()};

    // Find function return type
    LLVMTypeRef return_type = find_return_type(fun, &types_table);
    const LLVMTypeRef funcType =
        LLVMFunctionType(return_type, NULL, 0, 0);  // TODO: Add more args
    const LLVMValueRef func = LLVMAddFunction(*module, fun->name, funcType);
    const LLVMBasicBlockRef entry = LLVMAppendBasicBlock(func, "entry");
    const LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMPositionBuilderAtEnd(builder, entry);

    const struct Node* node = fun->body->body;
    struct Variable* variables = NULL;
    while (node != NULL) {
        switch (node->type) {
            case NO_REASSIGN_INT: {
                llvm_var_reassign_number(node, &variables, builder,
                                         types_table.int_type);
                break;
            }

            case NO_REASSIGN_FLOAT: {
                llvm_var_reassign_float(node, &variables, builder,
                                        types_table.float_type);
                break;
            }

            case NO_REASSIGN_CHAR: {
                llvm_var_reassign_number(node, &variables, builder,
                                         types_table.char_type);
                break;
            }

            case NO_ASSIGN_INT: {
                llvm_var_assign_number(node, &variables, builder,
                                       types_table.int_type);
                break;
            }

            case NO_ASSIGN_FLOAT: {
                llvm_var_assign_float(node, &variables, builder,
                                      types_table.float_type);
                break;
            }

            case NO_ASSIGN_CHAR: {
                llvm_var_assign_number(node, &variables, builder,
                                       types_table.char_type);
                break;
            }

            case NO_PRINT: {
                const LLVMValueRef str =
                    LLVMBuildGlobalStringPtr(builder, node->name, "str_const");
                LLVMTypeRef charPtrType = LLVMPointerType(LLVMInt8Type(), 0);
                const LLVMTypeRef putsType =
                    LLVMFunctionType(types_table.int_type, &charPtrType, 1, 0);
                LLVMValueRef putsFunc = LLVMGetNamedFunction(*module, "puts");
                if (!putsFunc) {
                    putsFunc = LLVMAddFunction(*module, "puts", putsType);
                }
                LLVMValueRef args[] = {str};
                LLVMBuildCall2(builder, putsType, putsFunc, args, 1, "");
                break;
            }

            case NO_RETURN: {
                llvm_return_generate_return(builder, variables, fun, node,
                                            types_table);
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

