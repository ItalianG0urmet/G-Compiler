#include "var.h"

#include <stdlib.h>
#include <string.h>

#include "ast_types.h"
#include "error_utils.h"
#include "lexer.h"

#define PEEK(i) (tokens[*current_index + (i)])

// -- assign --
struct Node* var_assign_int(const struct Token* tokens, int* current_index,
                            struct Node* node) {
    if (PEEK(0).type == TOKEN_INT) {
        if (PEEK(1).type == TOKEN_IDENTIFIER) {
            if (PEEK(2).type == TOKEN_END) {
                strncpy(node->name, PEEK(1).value, sizeof(node->name));
                node->type = NO_ASSIGN_INT;
                *current_index += 3;
                return node;
            }
            if (PEEK(2).type == TOKEN_ASSIGN) {
                strncpy(node->name, PEEK(1).value, sizeof(node->name));
                int val;
                // Positive number
                if (PEEK(3).type == TOKEN_INT &&
                    strcmp(PEEK(3).value, "int") != 0) {
                    val = atoi(PEEK(3).value);
                    *current_index += 5;
                }
                // Negative number
                else if (PEEK(3).type == TOKEN_MINUS &&
                         PEEK(4).type == TOKEN_INT &&
                         strcmp(PEEK(4).value, "int") != 0) {
                    val = -atoi(PEEK(4).value);
                    *current_index += 6;
                } else {
                    send_syntax_error(&PEEK(0), "Invalid initializer for int");
                    exit(1);
                }
                node->type = NO_ASSIGN_INT;
                node->number = val;
                return node;
            }
            send_syntax_error(&PEEK(0), "Invalid int declaration");
            exit(1);
        }
    }
    return NULL;
}

struct Node* var_assign_char(const struct Token* tokens, int* current_index,
                             struct Node* node) {
    if (PEEK(0).type == TOKEN_CHAR && PEEK(1).type == TOKEN_IDENTIFIER) {
        if (PEEK(2).type == TOKEN_END) {
            strncpy(node->name, PEEK(1).value, sizeof(node->name));
            node->type = NO_ASSIGN_CHAR;
            *current_index += 3;
            return node;
        }
        if (PEEK(2).type == TOKEN_ASSIGN && PEEK(3).type == TOKEN_LETTER) {
            strncpy(node->name, PEEK(1).value, sizeof(node->name));
            node->type = NO_ASSIGN_CHAR;
            node->letter = PEEK(3).value[0];
            *current_index += 5;
            return node;
        }
        send_syntax_error(&PEEK(0), "Syntax error in char declaration");
        exit(1);
    }
    return NULL;
}

struct Node* var_assign_float(const struct Token* tokens, int* current_index,
                              struct Node* node) {
    if (PEEK(0).type == TOKEN_FLOAT) {
        if (PEEK(1).type == TOKEN_IDENTIFIER) {
            if (PEEK(2).type == TOKEN_END) {
                strncpy(node->name, PEEK(1).value, sizeof(node->name));
                node->type = NO_ASSIGN_FLOAT;
                *current_index += 3;
                return node;
            }
            if (PEEK(2).type == TOKEN_ASSIGN) {
                strncpy(node->name, PEEK(1).value, sizeof(node->name));
                double val;
                // Positive number
                if (PEEK(3).type == TOKEN_FLOAT &&
                    strcmp(PEEK(3).value, "float") != 0) {
                    val = atof(PEEK(3).value);
                    *current_index += 5;
                }
                // Negative number
                else if (PEEK(3).type == TOKEN_MINUS &&
                         PEEK(4).type == TOKEN_FLOAT &&
                         strcmp(PEEK(4).value, "float") != 0) {
                    val = -atof(PEEK(4).value);
                    *current_index += 6;
                } else {
                    send_syntax_error(&PEEK(0),
                                      "Invalid initializer for float");
                    exit(1);
                }
                node->type = NO_ASSIGN_FLOAT;
                node->floating = val;
                return node;
            }
            send_syntax_error(&PEEK(0), "Malformed float declaration");
            exit(1);
        }
    }
    return NULL;
}

struct Node* var_assign_let(const struct Token* tokens, int* current_index,
                            struct Node* node) {
    if (PEEK(0).type != TOKEN_LET) {
        return NULL;
    }

    if (PEEK(1).type != TOKEN_IDENTIFIER) {
        send_syntax_error(&PEEK(1), "Expected identifier after 'let'");
        exit(1);
    }

    strncpy(node->name, PEEK(1).value, sizeof(node->name));

    if (PEEK(2).type != TOKEN_ASSIGN) {
        send_syntax_error(&PEEK(2), "Expected '=' after identifier");
        exit(1);
    }

    const struct Token* value_token = &PEEK(3);

    // INT assignment
    if (value_token->type == TOKEN_INT &&
        strcmp(value_token->value, "int") != 0) {
        node->type = NO_ASSIGN_INT;
        node->number = atoi(value_token->value);
        *current_index += 5;
        return node;
    }

    // Negative INT
    if (value_token->type == TOKEN_MINUS && PEEK(4).type == TOKEN_INT &&
        strcmp(PEEK(4).value, "int") != 0) {
        node->type = NO_ASSIGN_INT;
        node->number = -atoi(PEEK(4).value);
        *current_index += 6;
        return node;
    }

    // FLOAT assignment
    if (value_token->type == TOKEN_FLOAT &&
        strcmp(value_token->value, "float") != 0) {
        node->type = NO_ASSIGN_FLOAT;
        node->floating = atof(value_token->value);
        *current_index += 5;
        return node;
    }

    // Negative FLOAT
    if (value_token->type == TOKEN_MINUS && PEEK(4).type == TOKEN_FLOAT &&
        strcmp(PEEK(4).value, "float") != 0) {
        node->type = NO_ASSIGN_FLOAT;
        node->floating = -atof(PEEK(4).value);
        *current_index += 6;
        return node;
    }

    // CHAR assignment
    if (value_token->type == TOKEN_LETTER) {
        node->type = NO_ASSIGN_CHAR;
        node->letter = value_token->value[0];
        *current_index += 5;
        return node;
    }

    send_syntax_error(value_token,
                      "Unsupported value type in 'let' assignment");
    exit(1);
}

// -- reassign --
struct Node* var_reassign_int(const struct Token* tokens, int* current_index,
                              struct Node* node) {
    if (PEEK(0).type == TOKEN_IDENTIFIER && PEEK(1).type == TOKEN_ASSIGN) {
        strncpy(node->name, PEEK(0).value, sizeof(node->name));
        int val;
        if (PEEK(2).type == TOKEN_INT && strcmp(PEEK(2).value, "int") != 0 &&
            PEEK(3).type == TOKEN_END) {
            val = atoi(PEEK(2).value);
            *current_index += 4;
        } else if (PEEK(2).type == TOKEN_MINUS && PEEK(3).type == TOKEN_INT &&
                   strcmp(PEEK(3).value, "int") != 0 &&
                   PEEK(4).type == TOKEN_END) {
            val = -atoi(PEEK(3).value);
            *current_index += 5;
        } else {
            return NULL;
        }
        node->type = NO_REASSIGN_INT;
        node->number = val;
        return node;
    }
    return NULL;
}

struct Node* var_reassign_float(const struct Token* tokens, int* current_index,
                                struct Node* node) {
    if (PEEK(0).type == TOKEN_IDENTIFIER && PEEK(1).type == TOKEN_ASSIGN) {
        strncpy(node->name, PEEK(0).value, sizeof(node->name));
        double val;
        if (PEEK(2).type == TOKEN_FLOAT &&
            strcmp(PEEK(2).value, "float") != 0 && PEEK(3).type == TOKEN_END) {
            val = atof(PEEK(2).value);
            *current_index += 4;
        } else if (PEEK(2).type == TOKEN_MINUS && PEEK(3).type == TOKEN_FLOAT &&
                   strcmp(PEEK(3).value, "float") != 0 &&
                   PEEK(4).type == TOKEN_END) {
            val = -atof(PEEK(3).value);
            *current_index += 5;
        } else {
            return NULL;
        }
        node->type = NO_REASSIGN_FLOAT;
        node->floating = val;
        return node;
    }
    return NULL;
}

struct Node* var_reassign_char(const struct Token* tokens, int* current_index,
                               struct Node* node) {
    if (PEEK(0).type == TOKEN_IDENTIFIER && PEEK(1).type == TOKEN_ASSIGN &&
        PEEK(2).type == TOKEN_LETTER && PEEK(3).type == TOKEN_END) {
        strncpy(node->name, PEEK(0).value, sizeof(node->name));
        node->type = NO_REASSIGN_CHAR;
        node->letter = PEEK(2).value[0];
        *current_index += 4;
        return node;
    }
    return NULL;
}
