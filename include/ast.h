#ifndef AST_H
#define AST_H

#include "ast_types.h"

function_t* get_function_by_name(const char* name, function_list_t function_list);
argument_t* parse_param(const token_t* tokens, int first_param_index);
function_t parse_function(const token_t* tokens, int* current_index);
void add_function_to_list(function_t* fun, function_list_t* function_list);

#endif
