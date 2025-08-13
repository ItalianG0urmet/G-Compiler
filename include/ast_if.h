#ifndef IF_H
#define IF_H

struct Token;

struct Node* transform_into_node(const struct Token* tokens, int* current_index);
struct Node* parse_if(const struct Token* tokens, int* current_index);


#endif
