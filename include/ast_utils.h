#ifndef AST_UTILS_H
#define AST_UTILS_H

#include "ast_types.h"

#define EXPECT(tokens, idx, expected, msg)                        \
    do {                                                          \
        if ((tokens)[idx].type != (expected))                     \
            token_failed(tokens, idx, (msg), __FILE__, __LINE__); \
    } while (0)

__attribute__((noreturn, cold)) void token_failed(const struct Token* tokens,
                                                         int index,
                                                         const char* msg,
                                                         const char* file,
                                                         int line);

__attribute__((noreturn, cold)) void allocation_failed(const int line);

void check_if_allocated(const void* ptr, const int line);

#endif
