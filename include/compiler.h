#ifndef COMPILER_H
#define COMPILER_H

#include <llvm-c/BitWriter.h>
#include <stdbool.h>

#include "lexer_utils.h"
#include "llvm.h"

void compile_file(const char* file_path, const char* file_output, bool debug_active);

#endif
