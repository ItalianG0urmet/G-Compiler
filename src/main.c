#include <llvm-c/BitWriter.h>

#include "compiler.h"
#include "args_manager.h"

int main(int argc, const char* argv[]) {

    struct Flags flags = get_arguments_flags(argc, argv);
    compile_file(flags.file_name, flags.output_name, flags.debug_active);

    return 0;
}
