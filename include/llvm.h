#ifndef LLVM_H
#define LLVM_H

#include <stdbool.h>
#include "../include/ast.h"

void generate_llvm(struct Function_list function_list, const char* output_file, bool debug_active);

#endif
