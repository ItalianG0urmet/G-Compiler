#ifndef LLVM_H
#define LLVM_H

#include <stdbool.h>
#include "ast_types.h"
#include <llvm-c/Types.h>

void generate_llvm(struct Function_list function_list, const char* output_file, bool debug_active);

#endif
