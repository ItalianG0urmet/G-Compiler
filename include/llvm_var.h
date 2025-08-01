#ifndef LLVM_VAR_H
#define LLVM_VAR_H

#include <llvm-c/Core.h>
#include "ast_types.h"

struct Variable {
    char name[64];
    LLVMValueRef ref;
    LLVMTypeRef type;
    struct Variable* next;
};

LLVMValueRef find_variable(struct Variable* head, const char* name);

/* Assign */
void llvm_var_assign_number(const struct Node* node, struct Variable** variables,
                         const LLVMBuilderRef builder,
                         const LLVMTypeRef int_type);

void llvm_var_assign_float(const struct Node* node, struct Variable** variables,
                           const LLVMBuilderRef builder,
                           const LLVMTypeRef float_type);

/* Reassign */
void llvm_var_reassign_number(const struct Node* node, struct Variable** variables,
                           const LLVMBuilderRef builder,
                           const LLVMTypeRef int_type);

void llvm_var_reassign_float(const struct Node* node,
                             struct Variable** variables,
                             const LLVMBuilderRef builder,
                             const LLVMTypeRef float_type);

#endif
