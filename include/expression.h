#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ast_types.h"
#include "lexer.h"

struct Node* parse_expression(const struct Token* tokens,
                                const int* current_index);

#endif
