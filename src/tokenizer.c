#include "../include/tokenizer.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// -- Check the next char --
static int fpeek(FILE* file) {
    const int c = fgetc(file);
    if (c != EOF) ungetc(c, file);
    return c;
}

// -- Assign the right token --
static token_t first_token(const char buffer[256], char current) {
    token_t temp;

    if (strcmp(buffer, "int") == 0) {
        temp.type = TOKEN_INT;
    } else if (strcmp(buffer, "char") == 0) {
        temp.type = TOKEN_CHAR;
    } else if (strcmp(buffer, "float") == 0) {
        temp.type = TOKEN_FLOAT;
    } else if (strcmp(buffer, "void") == 0) {
        temp.type = TOKEN_VOID;
    } else {
        bool is_numeric = true;
        bool has_decimal_point = false;
        bool has_digits_before = false;
        bool has_digits_after = false;

        for (int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] == '.') {
                if (has_decimal_point) {
                    is_numeric = false;
                    break;
                }
                has_decimal_point = true;
            } else if (!isdigit(buffer[i])) {
                is_numeric = false;
                break;
            } else {
                if (has_decimal_point) {
                    has_digits_after = true;
                } else {
                    has_digits_before = true;
                }
            }
        }

        if (is_numeric) {
            if (has_decimal_point && (has_digits_before || has_digits_after)) {
                temp.type = TOKEN_FLOAT;
            } else if (!has_decimal_point) {
                temp.type = TOKEN_INT;
            } else {
                is_numeric = false;
            }
        }
        if (!is_numeric) {
            temp.type = TOKEN_IDENTIFIER;
        }
    }

    strcpy(temp.value, buffer);
    return temp;
}

// -- Allocate the token in the list --
static void push_token(token_t** tokens, const token_t* token,
                       int* tokens_capacity, int* tokens_count) {
    if (*tokens_count >= *tokens_capacity) {
        *tokens_capacity *= 2;
        token_t* new_tokens =
            realloc(*tokens, sizeof(token_t) * (*tokens_capacity));
        if (!new_tokens) {
            perror("[-] Can't allocate new space \n");
            exit(1);
        }
        *tokens = new_tokens;
    }
    (*tokens)[*tokens_count] = *token;
    (*tokens_count)++;
}

// -- Translate the toke type to a string --
static const char* token_type_to_string(const token_type_t type) {
    switch (type) {
        case TOKEN_EQUAL:
            return "EQUAL";
        case TOKEN_PLUS:
            return "PLUS";
        case TOKEN_MINUS:
            return "MINUS";
        case TOKEN_BITWISE_AND:
            return "BITWISE AND";
        case TOKEN_AND:
            return "AND";
        case TOKEN_BITWISE_OR:
            return "BITWISE OR";
        case TOKEN_INCREMENT:
            return "INCREMENT";
        case TOKEN_DOT:
            return "DOT";
        case TOKEN_OR:
            return "OR";
        case TOKEN_EMINOR:
            return "EMINOR";
        case TOKEN_EMAJOR:
            return "EMAJOR";
        case TOKEN_NEQUAL:
            return "NEQUAL";
        case TOKEN_DIVISION:
            return "DIVISION";
        case TOKEN_ASTERISK:
            return "ASTERISK";
        case TOKEN_NOT:
            return "NOT";
        case TOKEN_MAJOR:
            return "MAJOR";
        case TOKEN_MINOR:
            return "MINOR";
        case TOKEN_TEXT:
            return "TEXT";
        case TOKEN_INT:
            return "INT";
        case TOKEN_FLOAT:
            return "FLOAT";
        case TOKEN_CHAR:
            return "CHAR";
        case TOKEN_VOID:
            return "VOID";
        case TOKEN_LETTER:
            return "LETTER";
        case TOKEN_IDENTIFIER:
            return "IDENTIFIER";
        case TOKEN_ASSIGN:
            return "ASSIGN";
        case TOKEN_LPAREN:
            return "LPAREN";
        case TOKEN_RPAREN:
            return "RPAREN";
        case TOKEN_RBRACE:
            return "RBRACE";
        case TOKEN_LBRACE:
            return "LBRACE";
        case TOKEN_COMMA:
            return "COMMA";
        case TOKEN_END:
            return "END";
        case TOKEN_OEF:
            return "OEF";
        default:
            return "UNKNOWN";
    }
}

// -- Main function of the token class --
token_t* tokenizer(FILE* file) {
    int tokens_capacity = 16;
    int tokens_count = 0;
    token_t* tokens = malloc(sizeof(token_t) * tokens_capacity);
    if (tokens == NULL) {
        fprintf(stderr, "[-] Can't allocate memory for token");
        exit(1);
    }

    char buffer[256];
    int count = 0;
    buffer[0] = '\0';

    int current;
    while ((current = fgetc(file)) != EOF) {
        // -- TEXT  --
        if (current == '"') {
            token_t temp = {0};
            temp.type = TOKEN_TEXT;
            int i = 0;

            current = fgetc(file);
            while (current != '"' && current != EOF &&
                   i < sizeof(temp.value) - 1) {
                temp.value[i++] = (char)current;
                current = fgetc(file);
            }
            temp.value[i] = '\0';

            push_token(&tokens, &temp, &tokens_capacity, &tokens_count);
            continue;
        }

        // -- Letter --
        if (current == '\'') {
            token_t temp = {0};
            temp.type = TOKEN_LETTER;
            int i = 0;

            current = fgetc(file);
            while (current != '\'' && current != EOF &&
                   i < sizeof(temp.value) - 1) {
                temp.value[i++] = (char)current;
                current = fgetc(file);
            }
            temp.value[i] = '\0';

            push_token(&tokens, &temp, &tokens_capacity, &tokens_count);
            continue;
        }

        // -- Whitespace --
        if (isspace(current)) {
            if (count > 0) {
                buffer[count] = '\0';
                token_t temp = first_token(buffer, current);
                push_token(&tokens, &temp, &tokens_capacity, &tokens_count);
                count = 0;
                buffer[0] = '\0';
            }
            continue;
        }

        // -- Identifiers --
        if (isalpha(current) || current == '_') {
            if (count < sizeof(buffer) - 1) {
                buffer[count++] = (char)current;
            }
            continue;
        }

        // -- Digits --
        if (isdigit(current) || (current == '.' && isdigit(fpeek(file)))) {
            if (count < sizeof(buffer) - 1) {
                buffer[count++] = (char)current;
            }
            continue;
        }

        // -- Symbols --
        if (strchr("|=,;{}()>&<!*.+-/", current)) {
            if (count > 0) {
                buffer[count] = '\0';
                token_t temp = first_token(buffer, current);
                push_token(&tokens, &temp, &tokens_capacity, &tokens_count);
                count = 0;
                buffer[0] = '\0';
            }
            int next;
            token_t temp = {0};
            switch (current) {
                case '|':
                    next = fgetc(file);
                    if (next == '|') {
                        temp.type = TOKEN_OR;
                        strcpy(temp.value, "||");
                    } else {
                        temp.type = TOKEN_BITWISE_OR;
                        temp.value[0] = '|';
                        temp.value[1] = '\0';
                        ungetc(next, file);
                    }
                    break;
                case '/':
                    temp.type = TOKEN_DIVISION;
                    temp.value[0] = '/';
                    temp.value[1] = '\0';
                    break;
                case '+':
                    next = fgetc(file);
                    if (next == '+') {
                        temp.type = TOKEN_INCREMENT;
                        strcpy(temp.value, "++");
                    } else {
                        temp.type = TOKEN_PLUS;
                        temp.value[0] = '+';
                        temp.value[1] = '\0';
                        ungetc(next, file);
                    }
                    break;
                case '-':
                    temp.type = TOKEN_MINUS;
                    temp.value[0] = '-';
                    temp.value[1] = '\0';
                    break;
                case '.':
                    temp.type = TOKEN_DOT;
                    temp.value[0] = '.';
                    temp.value[1] = '\0';
                    break;
                case '>':
                    next = fgetc(file);
                    if (next == '=') {
                        temp.type = TOKEN_EMAJOR;
                        strcpy(temp.value, ">=");
                    } else {
                        temp.type = TOKEN_MAJOR;
                        temp.value[0] = '>';
                        temp.value[1] = '\0';
                        ungetc(next, file);
                    }
                    break;
                case '<':
                    next = fgetc(file);
                    if (next == '=') {
                        temp.type = TOKEN_EMINOR;
                        strcpy(temp.value, "<=");
                    } else {
                        temp.type = TOKEN_MINOR;
                        temp.value[0] = '<';
                        temp.value[1] = '\0';
                        ungetc(next, file);
                    }
                    break;
                case '=':
                    next = fgetc(file);
                    if (next == '=') {
                        temp.type = TOKEN_EQUAL;
                        strcpy(temp.value, "==");
                    } else {
                        temp.type = TOKEN_ASSIGN;
                        temp.value[0] = '=';
                        temp.value[1] = '\0';
                        ungetc(next, file);
                    }
                    break;
                case ';':
                    temp.type = TOKEN_END;
                    temp.value[0] = ';';
                    temp.value[1] = '\0';
                    break;
                case ',':
                    temp.type = TOKEN_COMMA;
                    temp.value[0] = ',';
                    temp.value[1] = '\0';
                    break;
                case '(':
                    temp.type = TOKEN_LPAREN;
                    temp.value[0] = '(';
                    temp.value[1] = '\0';
                    break;
                case ')':
                    temp.type = TOKEN_RPAREN;
                    temp.value[0] = ')';
                    temp.value[1] = '\0';
                    break;
                case '{':
                    temp.type = TOKEN_LBRACE;
                    temp.value[0] = '{';
                    temp.value[1] = '\0';
                    break;
                case '}':
                    temp.type = TOKEN_RBRACE;
                    temp.value[0] = '}';
                    temp.value[1] = '\0';
                    break;
                case '*':
                    temp.type = TOKEN_ASTERISK;
                    temp.value[0] = '*';
                    temp.value[1] = '\0';
                    break;
                case '!':
                    next = fgetc(file);
                    if (next == '=') {
                        temp.type = TOKEN_NEQUAL;
                        strcpy(temp.value, "!=");
                    } else {
                        temp.type = TOKEN_NOT;
                        temp.value[0] = '!';
                        temp.value[1] = '\0';
                        ungetc(next, file);
                    }
                    break;
                case '&':
                    next = fgetc(file);
                    if (next == '&') {
                        temp.type = TOKEN_AND;
                        strcpy(temp.value, "&&");
                    } else {
                        temp.type = TOKEN_BITWISE_AND;
                        temp.value[0] = '&';
                        temp.value[1] = '\0';
                        ungetc(next, file);
                    }
                    break;
                default:
                    temp.type = TOKEN_IDENTIFIER;
                    temp.value[0] = (char)current;
                    temp.value[1] = '\0';
                    break;
            }

            push_token(&tokens, &temp, &tokens_capacity, &tokens_count);
            continue;
        }

        fprintf(stderr, "[-] Invalid syntax %c \n", current);
        exit(1);
    }

    // If buffer has leftover data at EOF
    if (count > 0) {
        buffer[count] = '\0';
        token_t temp = first_token(buffer, '\0');
        push_token(&tokens, &temp, &tokens_capacity, &tokens_count);
        printf("[EOF-TOKEN] %s [%s]\n", temp.value,
               token_type_to_string(temp.type));
    }

    // Add OEF token
    token_t finish = {0};
    finish.type = TOKEN_OEF;
    strcpy(finish.value, "404");
    push_token(&tokens, &finish, &tokens_capacity, &tokens_count);

    // Print all tokens
    printf("------- FINAL TOKENS -------\n");
    for (int i = 0; i < tokens_count; i++) {
        printf("[%02d] %s, %s\n", i, tokens[i].value,
               token_type_to_string(tokens[i].type));
    }
    return tokens;
}

