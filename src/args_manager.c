#include "args_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

__attribute__((noreturn)) static void print_help_page(void) {
    bool use_colors = isatty(fileno(stdout));

    const char *RESET = use_colors ? "\033[0m" : "";
    const char *GREEN = use_colors ? "\033[32m" : "";
    const char *CYAN = use_colors ? "\033[36m" : "";
    const char *YELLOW = use_colors ? "\033[33m" : "";

    printf("%sUsage:%s compiler [options] <file>\n\n", CYAN, RESET);

    printf("%sOptions:%s\n", GREEN, RESET);
    printf("  %s--help%s          Display this help page and exit\n", YELLOW,
           RESET);
    printf("  %s-d%s              Enable debug mode\n", YELLOW, RESET);
    printf(
        "  %s-o <name>%s       Specify the output file name (default: "
        "output)\n",
        YELLOW, RESET);

    printf("\n%sExamples:%s\n", GREEN, RESET);
    printf("  compiler -d -o result.txt input.txt\n");
    printf("  compiler --help\n\n");
    exit(0);
}

struct Flags get_arguments_flags(int argc, const char *argv[]) {
    struct Flags flags = {0};
    flags.debug_active = false;
    strncpy(flags.output_name, "output", sizeof(flags.output_name) - 1);

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help_page();
        } else if (strcmp(argv[i], "-d") == 0) {
            flags.debug_active = true;
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            strncpy(flags.output_name, argv[++i],
                    sizeof(flags.output_name) - 1);
        } else {
            strncpy(flags.file_name, argv[i], sizeof(flags.file_name) - 1);
        }
    }

    return flags;
}
