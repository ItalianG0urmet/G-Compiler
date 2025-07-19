#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ast_types.h"
#include "tokenizer.h"

node_t* parse_expression(const token_t* tokens,
                                const int* current_index);

#endif
