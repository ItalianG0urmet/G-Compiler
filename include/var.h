#ifndef VAR_H
#define VAR_H

struct Token;  // From lexer.h
struct Node;   // From ast_types.h

struct Node* var_assign_char(const struct Token* tokens, int* current_index,
                             struct Node* node);

struct Node* var_assign_int(const struct Token* tokens, int* current_index,
                            struct Node* node);

struct Node* var_assign_float(const struct Token* tokens, int* current_index,
                              struct Node* node);

struct Node* var_reassign_int(const struct Token* tokens, int* current_index,
                              struct Node* node);

struct Node* var_reassign_float(const struct Token* tokens, int* current_index,
                                struct Node* node);

struct Node* var_reassign_char(const struct Token* tokens, int* current_index,
                               struct Node* node);

struct Node* var_assign_let(const struct Token* tokens, int* current_index,
                            struct Node* node);
#endif
