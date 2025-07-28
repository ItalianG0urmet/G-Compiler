#ifndef LLVM_GENERATOR_H
#define LLVM_GENERATOR_H

#include <llvm-c/Core.h>
struct Function;

void llvm_generator_generate_function_ir(struct Function* fun, LLVMContextRef* context,
                                 const LLVMModuleRef* module);

#endif
