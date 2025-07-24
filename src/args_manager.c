#include "args_manager.h"

#include <string.h>

struct Flags get_arguments_flags(int argc, const char* argv[]) {
    struct Flags flags = {0};
    flags.debug_active = false;
    strncpy(flags.output_name, "output", sizeof(flags.output_name) - 1);

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            flags.debug_active = true;
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            strncpy(flags.output_name, argv[++i], sizeof(flags.output_name) - 1);
        } else {
            strncpy(flags.file_name, argv[i], sizeof(flags.file_name) - 1);
        }
    }

    return flags;
}
