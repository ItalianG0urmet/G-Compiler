#include "error_utils.h"

#include <stdbool.h>
#include <stdio.h>

#include "lexer.h"

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

static const char* RED;
static const char* RESET;

static void init_color_support(void) {
    bool use_colors = isatty(fileno(stderr));
    RED = use_colors ? "\033[31m" : "";
    RESET = use_colors ? "\033[0m" : "";
}

void send_syntax_error_by_line(const char* line, const int column,
                               const char* error) {
    init_color_support();

    fprintf(stderr,
            "\n%sError%s at column %d:\n"
            "  %s\n"
            "  %s\n",
            RED, RESET, column, error, line);
}

void send_syntax_error(const struct Token* token, const char* error) {
    if (!token) return;

    init_color_support();

    fprintf(stderr,
            "\n%sError%s at column %d:\n"
            "  %s\n"
            "  %s\n",
            RED, RESET, token->column, error, token->full_line);
}
