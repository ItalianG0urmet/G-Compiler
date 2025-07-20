#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ast_types.h"
#include "tokenizer.h"

struct Node* parse_expression(const token_t* tokens,
                                const int* current_index);

#endif
