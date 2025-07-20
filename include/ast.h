#ifndef AST_H
#define AST_H

#include "ast_types.h"
#include "ast_utils.h"
#include "if.h"
#include "expression.h"

struct Node* transform_into_node(const struct Token* tokens, int* current_index);
struct Function* get_function_by_name(const char* name, struct Function_list function_list);
struct Argument* parse_param(const struct Token* tokens, int first_param_index);
struct Function parse_function(const struct Token* tokens, int* current_index);
void add_function_to_list(struct Function* fun, struct Function_list* function_list);

#endif
