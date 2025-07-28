#ifndef LLVM_RETURN_H
#define LLVM_RETURN_H

#include <llvm-c/Core.h>
struct Variable;       // form llvm_var.h
struct Node;           // from ast_types.h
struct Function;       // from ast_types.h
struct LLVMTypeTable;  // from llvm_types.h

void llvm_return_generate_return(const LLVMBuilderRef builder,
                                 struct Variable* variables,
                                 struct Function* fun, const struct Node* node,
                                 struct LLVMTypeTable types_table);
#endif
