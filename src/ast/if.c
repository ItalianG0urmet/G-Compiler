#include "../../include/if.h"

// TODO
static node_t* parse_expression(const token_t* tokens, int* current_index){


}

node_t* parse_if(const token_t* tokens, int* current_index) {
    node_t* node = calloc(1, sizeof(node_t));
    check_if_allocated(node, __LINE__);

    node->type = NO_IF;

    // Syntax check
    EXPECT(tokens, *current_index + 1, TOKEN_LPAREN, "expected '(' after 'if'");

    *current_index += 2;
    node->lnode = parse_expression(tokens, current_index);

    EXPECT(tokens, *current_index, TOKEN_RPAREN, "expected ')' after condition");
    (*current_index)++;

    EXPECT(tokens, *current_index, TOKEN_LBRACE, "expected '{' after if()");
    (*current_index)++;

    // Generate all the nodes
    node_t* then_body = calloc(1, sizeof(node_t));
    check_if_allocated(then_body, __LINE__);
    then_body->type = NO_BODY;
    node_t* then_last = NULL;
    while (tokens[*current_index].type != TOKEN_RBRACE &&
           tokens[*current_index].type != TOKEN_OEF) {
        node_t* stmt = transform_into_node(tokens, current_index);
        if (!then_body->body) {
            then_body->body = stmt;
        } else {
            if (then_last == NULL) {
                fprintf(stderr, "[-] then_last is null\n");
                exit(1);
            }
            then_last->next = stmt;
        }
        then_last = stmt;
    }

    // Syntax check
    EXPECT(tokens, *current_index, TOKEN_RBRACE,
           "expected '}' to close if body");
    (*current_index)++;

    node->then = then_body;
    return node;
}


