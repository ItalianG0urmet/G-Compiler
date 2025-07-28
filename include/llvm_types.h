#ifndef LLVM_TYPES_H
#define LLVM_TYPES_H

#include <llvm-c/Types.h>

struct LLVMTypeTable {
    LLVMTypeRef void_type;
    LLVMTypeRef int_type;
    LLVMTypeRef float_type;
    LLVMTypeRef char_type;
};


#endif
