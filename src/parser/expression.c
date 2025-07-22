#include "../../include/expression.h"

static int get_node_position(enum Token_type type) {
    if (type == TOKEN_ASTERISK || type == TOKEN_DIVISION) return 1;
    if (type == TOKEN_PLUS || type == TOKEN_MINUS) return 2;
    return -1;
}

struct Node* parse_expression(const struct Token* tokens,
                                const int* current_index) {
    // TODO: Create expression parser
    if (tokens[(*current_index + 1)].type != TOKEN_LPAREN) {
    }
    return NULL;  // TODO
}

