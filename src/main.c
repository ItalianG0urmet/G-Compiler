#include <llvm-c/BitWriter.h>

#include "../include/compiler.h"

int main(int argc, const char* argv[]) {

    compile_file(argv[1]);

    return 0;
}
