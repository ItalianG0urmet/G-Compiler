#ifndef LLVM_H
#define LLVM_H

#include <stdbool.h>
struct Function_list;

void generate_llvm(struct Function_list function_list, const char* output_file, bool debug_active);

#endif
