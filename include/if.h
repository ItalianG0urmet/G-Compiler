#ifndef IF_H
#define IF_H

#include "ast_types.h"
#include "ast_utils.h"
#include <stdlib.h>

struct Node* transform_into_node(const token_t* tokens, int* current_index);
struct Node* parse_if(const token_t* tokens, int* current_index);


#endif
