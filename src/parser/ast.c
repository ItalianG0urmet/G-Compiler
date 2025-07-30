#include "ast.h"

#include <stdlib.h>
#include <string.h>

#include "ast_utils.h"
#include "error_utils.h"
#include "if.h"
#include "lexer.h"
#include "var.h"

#define PEEK(i) (tokens[*current_index + (i)])

#define TRY_PARSE(f)                                      \
    do {                                                  \
        struct Node* _n = f(tokens, current_index, node); \
        if (_n != NULL) return _n;                        \
    } while (0)

struct Node* transform_into_node(const struct Token* tokens,
                                 int* current_index) {
    struct Node* node = calloc(1, sizeof(struct Node));
    check_if_allocated(node, __LINE__);

    node->column = PEEK(0).column;
    strncpy(node->full_line, PEEK(0).full_line, sizeof(node->full_line));

    // TODO: If
    if (PEEK(0).type == TOKEN_IDENTIFIER && strcmp(PEEK(0).value, "if") == 0) {
        printf("[+] Found if \n");
        node = parse_if(tokens, current_index);
        return node;
    }

    // Var
    TRY_PARSE(var_assign_int);
    TRY_PARSE(var_assign_float);
    TRY_PARSE(var_assign_char);
    TRY_PARSE(var_reassign_int);
    TRY_PARSE(var_reassign_float);
    TRY_PARSE(var_reassign_char);

    // Return
    if (PEEK(0).type == TOKEN_IDENTIFIER &&
        strcmp(PEEK(0).value, "return") == 0) {
        if (PEEK(1).type == TOKEN_INT && strcmp(PEEK(1).value, "int") != 0) {
            const int return_code = atoi(PEEK(1).value);
            node->type = NO_RETURN;
            node->number = return_code;
            *current_index += 3;
            return node;
        } else if (PEEK(1).type == TOKEN_IDENTIFIER) {
            node->type = NO_RETURN;
            strncpy(node->name, PEEK(1).value, sizeof(node->name));
            *current_index += 3;
            return node;
        }

        send_syntax_error(&PEEK(0), "Invalid return statement");
        exit(1);
    }

    // Text (print)
    if (PEEK(0).type == TOKEN_TEXT && PEEK(-1).type != TOKEN_ASSIGN) {
        if (PEEK(1).type == TOKEN_END) {
            node->type = NO_PRINT;
            strncpy(node->name, PEEK(0).value, sizeof(node->name));
            *current_index += 2;
            return node;
        }
    }

    send_syntax_error(&PEEK(0), "Unknown or unsupported token");
    exit(1);
}

struct Function* get_function_by_name(
    const char* name, const struct Function_list function_list) {
    for (int i = 0; i < function_list.count; i++) {
        if (strcmp(function_list.functions[i]->name, name) == 0) {
            return function_list.functions[i];
        }
    }
    return NULL;
}

struct Function parse_function(const struct Token* tokens, int* current_index) {
    struct Token identifier = tokens[*current_index + 1];

    // Find return type
    enum Function_return_type return_type;
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
    struct Argument* args = parse_param(tokens, *current_index + 3);
    while (tokens[*current_index].type == TOKEN_LBRACE) {
        (*current_index)++;
    }

    struct Function fun;
    fun.arguments = args;
    fun.return_type = return_type;

    EXPECT(tokens, *current_index + 1, TOKEN_IDENTIFIER,
           "invalid function identifier");

    strncpy(fun.name, identifier.value, sizeof(fun.name));

    // Skip to {
    while (tokens[*current_index].type != TOKEN_LBRACE) {
        (*current_index)++;
    }
    (*current_index)++;

    // Start node transformation
    struct Node* body = calloc(1, sizeof(struct Node));
    check_if_allocated(body, __LINE__);
    body->type = NO_BODY;
    struct Node* last = NULL;
    int node_count = 0;
    while (tokens[*current_index].type != TOKEN_RBRACE &&
           tokens[*current_index].type != TOKEN_OEF) {
        struct Node* node = transform_into_node(tokens, current_index);

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
    (*current_index)++;
    return fun;
}

struct Argument* parse_param(const struct Token* tokens,
                             const int first_param_index) {
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

    struct Argument* arguments = NULL;
    if (arg_count > 0) {
        arguments = calloc(arg_count, sizeof(struct Argument));
        check_if_allocated(arguments, __LINE__);
    }

    index = first_param_index;
    int arg_index = 0;
    while (tokens[index].type != TOKEN_RPAREN &&
           tokens[index].type != TOKEN_OEF) {
        struct Argument arg;

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

void add_function_to_list(struct Function* fun,
                          struct Function_list* function_list) {
    function_list->functions =
        realloc(function_list->functions,
                sizeof(struct Function*) * (function_list->count + 1));

    if (!function_list->functions) {
        fprintf(stderr, "[-] Error reallocating memory for function list\n");
        exit(1);
    }

    function_list->functions[function_list->count] = fun;
    function_list->count++;
}
