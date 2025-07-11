#include "../include/ast.h"

#include <stdlib.h>
#include <string.h>

// -- Syntax checking macro --
#define EXPECT(token_arr, index, expectedType, msg)                \
    if ((token_arr)[index].type != expectedType) {                 \
        fprintf(stderr, "[-] Syntax Error: %s. Found '%s'\n", msg, \
                (token_arr)[index].value);                         \
        exit(1);                                                   \
    }

// -- Check if a ptr in allocated --
static void check_if_allocated(const void* ptr, const int line) {
    if (ptr == NULL) {
        fprintf(stderr, "[-] Failed to allocate memory, at line %d of %s\n",
                line, __FILE__);
        exit(1);
    }
}

// -- Parse the symbols and expression --
static node_t* parse_expression(const token_t* tokens,
                                const int* current_index) {
    // TODO: Create expression parser
    if (tokens[(*current_index + 1)].type != TOKEN_LPAREN) {
    }
    return NULL;  // TODO
}

static node_t* parse_if(const token_t* tokens, int* current_index);

// -- Transform some tokens into node --
static node_t* transform_into_node(const token_t* tokens, int* current_index) {
    node_t* node = calloc(1, sizeof(node_t));
    check_if_allocated(node, __LINE__);

    // If
    if (tokens[*current_index].type == TOKEN_IDENTIFIER &&
        strcmp(tokens[*current_index].value, "if") == 0) {
        printf("[+] Found if \n");
        node = parse_if(tokens, current_index);
        return node;
    }

    // Float & not init
    if (tokens[*current_index].type == TOKEN_FLOAT) {
        if (tokens[*current_index + 1].type == TOKEN_IDENTIFIER) {
            if (tokens[*current_index + 2].type == TOKEN_END) {
                strncpy(node->name, tokens[*current_index + 1].value,
                        sizeof(node->name));
                node->type = NO_ASSIGN_FLOAT;
                printf("[+] Added not defined float named %s \n", node->name);
                *current_index += 3;
                return node;
            }

            if (tokens[*current_index + 2].type == TOKEN_ASSIGN &&
                tokens[*current_index + 3].type == TOKEN_FLOAT &&
                strcmp(tokens[*current_index + 3].value, "float") != 0) {
                strncpy(node->name, tokens[*current_index + 1].value,
                        sizeof(node->name));
                node->type = NO_ASSIGN_FLOAT;
                node->floating = atof(tokens[*current_index + 3].value);
                printf("[+] Added defined float named %s with value %f\n",
                       node->name, node->floating);
                *current_index += 5;
                return node;
            }

            fprintf(stderr, "[-] Syntax error in float definition\n");
            exit(1);
        }
    }

    // Int & not init
    if (tokens[*current_index].type == TOKEN_INT) {
        if (tokens[*current_index + 1].type == TOKEN_IDENTIFIER) {
            if (tokens[*current_index + 2].type == TOKEN_END) {
                strncpy(node->name, tokens[*current_index + 1].value,
                        sizeof(node->name));
                node->type = NO_ASSIGN_INT;
                printf("[+] Added not defined int named %s \n", node->name);
                *current_index += 3;
                return node;
            }
            if (tokens[*current_index + 2].type == TOKEN_ASSIGN) {
                if (tokens[*current_index + 3].type == TOKEN_INT &&
                    strcmp(tokens[*current_index + 3].value, "int") != 0) {
                    strncpy(node->name, tokens[*current_index + 1].value,
                            sizeof(node->name));
                    node->type = NO_ASSIGN_INT;
                    node->number = atoi(tokens[*current_index + 3].value);
                    printf("[+] Added defined int named %s \n", node->name);
                    *current_index += 5;
                    return node;
                }
                fprintf(stderr, "[-] Syntax error \n");
                exit(1);
            }
            fprintf(stderr, "[-] Invalid int \n");
            exit(1);
        }
    }

    // Increment
    if (tokens[*current_index].type == TOKEN_IDENTIFIER &&
        tokens[*current_index + 1].type == TOKEN_INCREMENT &&
        tokens[*current_index + 2].type == TOKEN_END) {
        node->type = NO_INCREMENT;
        printf("[+] Added increment for %s \n", tokens[*current_index].value);
        *current_index += 3;
        return node;
    }

    // Char
    if (tokens[*current_index].type == TOKEN_CHAR &&
        tokens[*current_index + 1].type == TOKEN_IDENTIFIER) {
        if (tokens[*current_index + 2].type == TOKEN_END) {
            strncpy(node->name, tokens[*current_index + 1].value,
                    sizeof(node->name));
            node->type = NO_ASSIGN_CHAR;
            printf("[+] Added not defined char named %s \n", node->name);
            *current_index += 3;
            return node;
        }
        if (tokens[*current_index + 2].type == TOKEN_ASSIGN &&
            tokens[*current_index + 3].type == TOKEN_LETTER) {
            strncpy(node->name, tokens[*current_index + 1].value,
                    sizeof(node->name));
            node->type = NO_ASSIGN_CHAR;
            node->letter = tokens[*current_index + 3].value[0];
            printf("[+] Added defined char named %s with value '%c'\n",
                   node->name, node->letter);
            *current_index += 5;
            return node;
        }
        fprintf(stderr, "[-] Syntax error in char definition\n");
        exit(1);
    }

    // Return
    if (tokens[*current_index].type == TOKEN_IDENTIFIER &&
        strcmp(tokens[*current_index].value, "return") == 0) {
        if (tokens[*current_index + 1].type == TOKEN_INT &&
            strcmp(tokens[*current_index + 1].value, "int") != 0) {
            const int return_code = atoi(tokens[*current_index + 1].value);
            node->type = NO_RETURN;
            node->number = return_code;
            printf("[+] Added return type, that return %d \n", return_code);
            *current_index += 3;
            return node;
        }
        fprintf(stderr, "[-] Invalid syntax on return function \n");
        exit(1);
    }

    // Text
    if (tokens[*current_index].type == TOKEN_TEXT &&
        tokens[*current_index - 1].type != TOKEN_ASSIGN) {
        if (tokens[*current_index + 1].type == TOKEN_END) {
            node->type = NO_PRINT;
            strncpy(node->name, tokens[*current_index].value,
                    sizeof(node->name));
            *current_index += 2;
            printf("[+] Added text type, that contains \"%s\" \n", node->name);
            return node;
        }
    }

    fprintf(stderr,
            "[-] Unknown or unsupported token at index %d (type=%d, "
            "value='%s')\n",
            *current_index, tokens[*current_index].type,
            tokens[*current_index].value);
    exit(1);
}

// -- Parse the if statement
static node_t* parse_if(const token_t* tokens, int* current_index) {
    node_t* node = calloc(1, sizeof(node_t));
    check_if_allocated(node, __LINE__);

    node->type = NO_IF;

    // Syntax check
    EXPECT(tokens, *current_index + 1, TOKEN_LPAREN, "expected '(' after 'if'")

    *current_index += 2;
    node->lnode = parse_expression(tokens, current_index);

    EXPECT(tokens, *current_index, TOKEN_RPAREN, "expected ')' after condition")
    (*current_index)++;

    EXPECT(tokens, *current_index, TOKEN_LBRACE, "expected '{' after if()")
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
           "expected '}' to close if body")
    (*current_index)++;

    // Attach
    node->then = then_body;
    return node;
}

// -- Get a function by name --
function_t* get_function_by_name(const char* name,
                                 const function_list_t function_list) {
    for (int i = 0; i < function_list.count; i++) {
        if (strcmp(function_list.functions[i]->name, name) == 0) {
            return function_list.functions[i];
        }
    }
    return NULL;
}

// -- Create a function --
function_t parse_function(const token_t* tokens, int* current_index) {
    token_t identifier = tokens[*current_index + 1];

    // Find return type
    fun_return_type_t return_type;
    switch (tokens[*current_index].type) {
        case TOKEN_VOID:
            return_type = RET_VOID;
            break;
        case TOKEN_INT:
            return_type = RET_INT;
            break;
        case TOKEN_CHAR:
            return_type = RET_CHAR;
            break;
        case TOKEN_FLOAT:
            return_type = RET_FLOAT;
            break;
        default:
            fprintf(stderr, "[-] Can't find return type of %s\n",
                    identifier.value);
            exit(1);
    }

    // Parse the args and define
    argument_t* args = parse_param(tokens, *current_index + 3);
    while (tokens[*current_index].type == TOKEN_LBRACE) {
        (*current_index)++;
    }

    function_t fun;
    fun.arguments = args;
    fun.return_type = return_type;

    EXPECT(tokens, *current_index + 1, TOKEN_IDENTIFIER,
           "invalid function identifier")

    strncpy(fun.name, identifier.value, sizeof(fun.name));

    // Skip to {
    while (tokens[*current_index].type != TOKEN_LBRACE) {
        (*current_index)++;
    }
    (*current_index)++;

    // Start node transformation
    node_t* body = calloc(1, sizeof(node_t));
    check_if_allocated(body, __LINE__);
    body->type = NO_BODY;
    node_t* last = NULL;
    int node_count = 0;
    printf("\n[+] Defining function %s [+]\n", fun.name);
    while (tokens[*current_index].type != TOKEN_RBRACE &&
           tokens[*current_index].type != TOKEN_OEF) {
        node_t* node = transform_into_node(tokens, current_index);

        if (body->body == NULL) {
            body->body = node;
        } else {
            if (last == NULL) {
                fprintf(stderr, "[-] Last is null \n");
                exit(1);
            }
            last->next = node;
        }

        last = node;
        node_count++;
    }

    if (last) {
        last->next = NULL;
    }

    fun.body = body;
    fun.node_count = node_count;
    printf("[*] %s function have %d nodes \n", fun.name, fun.node_count);
    (*current_index)++;
    return fun;
}

// Get args for function
argument_t* parse_param(const token_t* tokens, const int first_param_index) {
    int index = first_param_index;
    int arg_count = 0;

    while (tokens[index].type != TOKEN_RPAREN &&
           tokens[index].type != TOKEN_OEF) {
        if (tokens[index].type == TOKEN_INT ||
            tokens[index].type == TOKEN_FLOAT ||
            tokens[index].type == TOKEN_CHAR) {
            arg_count++;
        }

        index += 2;
        if (tokens[index].type == TOKEN_COMMA) {
            index++;
        }
    }

    argument_t* arguments = calloc(arg_count, sizeof(argument_t));
    check_if_allocated(arguments, __LINE__);

    index = first_param_index;
    int arg_index = 0;
    while (tokens[index].type != TOKEN_RPAREN &&
           tokens[index].type != TOKEN_OEF) {
        argument_t arg;

        switch (tokens[index].type) {
            case TOKEN_INT:
                arg.type = ARG_INT;
                break;
            case TOKEN_FLOAT:
                arg.type = ARG_FLOAT;
                break;
            case TOKEN_CHAR:
                arg.type = ARG_CHAR;
                break;
            default:
                fprintf(stderr, "[-] Invalid param: %s\n", tokens[index].value);
                exit(1);
        }

        index += 2;
        arguments[arg_index++] = arg;

        if (tokens[index].type == TOKEN_COMMA) {
            index++;
        }
    }

    return arguments;
}

void add_function_to_list(function_t* fun, function_list_t* function_list) {
    function_list->functions =
        realloc(function_list->functions,
                sizeof(function_t*) * (function_list->count + 1));

    if (!function_list->functions) {
        fprintf(stderr, "[-] Error reallocating memory for function list\n");
        exit(1);
    }

    function_list->functions[function_list->count] = fun;
    function_list->count++;
}
