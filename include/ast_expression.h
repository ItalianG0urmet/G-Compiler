#ifndef EXPRESSION_H
#define EXPRESSION_H

struct Token;

struct Node* parse_expression(const struct Token* tokens,
                                const int* current_index);

#endif
