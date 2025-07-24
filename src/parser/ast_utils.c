#include "ast_utils.h"

#include <stdlib.h>
#include "lexer.h"

__attribute__((noreturn, cold)) void token_failed(const struct Token* tokens,
                                                         int index,
                                                         const char* msg,
                                                         const char* file,
                                                         int line) {
    fprintf(stderr, "[-] Syntax Error: %s at %s:%d. Found '%s'\n", msg, file,
            line, tokens[index].value);
    exit(1);
}

__attribute__((noreturn, cold)) void allocation_failed(const int line) {
    fprintf(stderr, "[-] Failed to allocate memory, at line %d of %s\n", line,
            __FILE__);
    exit(1);
}

void check_if_allocated(const void* ptr, const int line) {
    if (ptr == NULL) {
        allocation_failed(line);
    }
}

