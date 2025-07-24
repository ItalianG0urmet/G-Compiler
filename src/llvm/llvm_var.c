#include "../../include/llvm_var.h"

#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_utils.h"

/* Var utils */
static struct Variable* add_variable(struct Variable** head, const char* name,
                                     LLVMValueRef ref, LLVMTypeRef type) {
    struct Variable* var = malloc(sizeof(struct Variable));
    if (!var) {
        fprintf(stderr, "[-] Failed to allocate memory for variable\n");
        exit(1);
    }
    strncpy(var->name, name, sizeof(var->name));
    var->ref = ref;
    var->next = *head;
    var->type = type;
    return var;
}

static LLVMValueRef find_variable(struct Variable* head, const char* name) {
    while (head) {
        if (strcmp(head->name, name) == 0) {
            return head->ref;
        }
        head = head->next;
    }
    return NULL;
}

static LLVMTypeRef find_variable_type(struct Variable* head, const char* name) {
    while (head) {
        if (strcmp(head->name, name) == 0) {
            return head->type;
        }
        head = head->next;
    }
    return NULL;
}

/* Assign */
void llvm_var_assign_number(const struct Node* node,
                            struct Variable** variables,
                            const LLVMBuilderRef builder,
                            const LLVMTypeRef type) {
    const LLVMValueRef var = LLVMBuildAlloca(builder, type, node->name);
    LLVMBuildStore(builder, LLVMConstInt(type, node->number, 0), var);
    *variables = add_variable(variables, node->name, var, type);
}

void llvm_var_assign_float(const struct Node* node, struct Variable** variables,
                           const LLVMBuilderRef builder,
                           const LLVMTypeRef float_type) {
    const LLVMValueRef var = LLVMBuildAlloca(builder, float_type, node->name);
    LLVMBuildStore(builder, LLVMConstReal(float_type, node->floating), var);
    *variables = add_variable(variables, node->name, var, float_type);
}

/* Reassign */
void llvm_var_reassign_number(const struct Node* node,
                              struct Variable** variables,
                              const LLVMBuilderRef builder,
                              const LLVMTypeRef type) {
    LLVMValueRef var = find_variable(*variables, node->name);
    LLVMTypeRef original_type = find_variable_type(*variables, node->name);
    if (!var) {
        send_syntax_error_by_line(node->full_line, node->column,
                                  "Variable used before declaration");
        exit(1);
    }
    if (original_type != type) {
        send_syntax_error_by_line(node->full_line, node->column,
                                  "Wrong delcaration for var");
        exit(1);
    }
    LLVMBuildStore(builder, LLVMConstInt(type, node->number, 0), var);
}

void llvm_var_reassign_float(const struct Node* node,
                             struct Variable** variables,
                             const LLVMBuilderRef builder,
                             const LLVMTypeRef float_type) {
    LLVMValueRef var = find_variable(*variables, node->name);
    LLVMTypeRef type = find_variable_type(*variables, node->name);
    if (!var) {
        send_syntax_error_by_line(node->full_line, node->column,
                                  "Variable used before declaration");
        exit(1);
    }
    if (type != float_type) {
        send_syntax_error_by_line(node->full_line, node->column,
                                  "Wrong delcaration for var");
        exit(1);
    }
    LLVMBuildStore(builder, LLVMConstReal(float_type, node->floating), var);
}

