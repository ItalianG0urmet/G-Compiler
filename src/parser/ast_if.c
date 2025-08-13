#include "ast_if.h"

#include <stdlib.h>
#include "ast_utils.h"
#include "ast_types.h"

// TODO
static struct Node* parse_expression(const struct Token* tokens, int* current_index){


}

struct Node* parse_if(const struct Token* tokens, int* current_index) {
    struct Node* node = calloc(1, sizeof(struct Node));
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
    struct Node* then_body = calloc(1, sizeof(struct Node));
    check_if_allocated(then_body, __LINE__);
    then_body->type = NO_BODY;
    struct Node* then_last = NULL;
    while (tokens[*current_index].type != TOKEN_RBRACE &&
           tokens[*current_index].type != TOKEN_OEF) {
        struct Node* stmt = transform_into_node(tokens, current_index);
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


