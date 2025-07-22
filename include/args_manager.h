#ifndef ARGS_MANAGER_H
#define ARGS_MANAGER_H

#include <stdbool.h>

struct Flags {
    bool debug_active;
    char output_name[256];
    char file_name[256];
};

struct Flags get_arguments_flags(int argc, const char* argv[]);

#endif
