#ifndef LLVM_H
#define LLVM_H

#include <stdbool.h>
#include "../include/ast.h"
#include <llvm-c/Analysis.h>


struct Variable {
    char name[64];
    LLVMValueRef ref;
    LLVMTypeRef type;
    struct Variable* next;
};

void generate_llvm(struct Function_list function_list, const char* output_file, bool debug_active);

#endif
