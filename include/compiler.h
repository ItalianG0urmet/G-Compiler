#ifndef COMPILER_H
#define COMPILER_H

#include <llvm-c/BitWriter.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "llvm.h"
#include "lexer.h"

void compile_file(const char* file_path);

#endif
