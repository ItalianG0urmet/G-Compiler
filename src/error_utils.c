#include "error_utils.h"

#include <stdio.h>
#include "lexer.h"

void send_syntax_error_by_line(const char* line, const int column,
                               const char* error) {
    fprintf(stderr,
            "\n\x1b[31mError\x1b[0m at line %d:\n"
            "  %s\n"
            "  %s\n",
            column, error, line);
}

void send_syntax_error(const struct Token* token, const char* error) {
    if (!token) return;

    fprintf(stderr,
            "\n\x1b[31mError\x1b[0m at line %d:\n"
            "  %s\n"
            "  %s\n",
            token->column, error, token->full_line);
}
