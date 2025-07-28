#include "llvm_return.h"

#include <stdlib.h>

#include "ast_types.h"
#include "error_utils.h"
#include "llvm_types.h"
#include "llvm_var.h"

void llvm_return_generate_return(const LLVMBuilderRef builder, struct Variable* variables,
                     struct Function* fun, const struct Node* node,
                     struct LLVMTypeTable types_table) {
    switch (fun->return_type) {
        case RET_VOID:
            LLVMBuildRetVoid(builder);
            break;

        case RET_INT: {
            if (node->name[0] != '\0') {
                LLVMValueRef var = find_variable(variables, node->name);
                if (!var) {
                    send_syntax_error_by_line(node->full_line, node->column,
                                              "Variable not declared");
                    exit(1);
                }
                LLVMValueRef loaded_val = LLVMBuildLoad2(
                    builder, types_table.int_type, var, "ret_int");
                LLVMBuildRet(builder, loaded_val);
            } else {
                LLVMBuildRet(builder, LLVMConstInt(types_table.int_type,
                                                    node->number, 0));
            }
            break;
        }

        case RET_FLOAT: {
            if (node->name[0] != '\0') {
                LLVMValueRef var = find_variable(variables, node->name);
                if (!var) {
                    send_syntax_error_by_line(node->full_line, node->column,
                                              "Variable not declared");
                    exit(1);
                }
                LLVMValueRef loaded_val = LLVMBuildLoad2(
                    builder, types_table.float_type, var, "ret_float");
                LLVMBuildRet(builder, loaded_val);
            } else {
                LLVMBuildRet(builder, LLVMConstReal(types_table.float_type,
                                                     node->floating));
            }
            break;
        }

        case RET_CHAR: {
            if (node->name[0] != '\0') {
                LLVMValueRef var = find_variable(variables, node->name);
                if (!var) {
                    send_syntax_error_by_line(node->full_line, node->column,
                                              "Variable not declared");
                    exit(1);
                }
                LLVMValueRef loaded_val = LLVMBuildLoad2(
                    builder, types_table.char_type, var, "ret_char");
                LLVMBuildRet(builder, loaded_val);
            } else {
                LLVMBuildRet(builder, LLVMConstInt(types_table.char_type,
                                                    node->letter, 0));
            }
            break;
        }

        default:
            send_syntax_error_by_line(node->full_line, node->column,
                                      "Invalid return type");
            exit(1);
    }
}
