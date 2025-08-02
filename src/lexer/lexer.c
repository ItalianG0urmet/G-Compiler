#include "lexer.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN_VALUE 256
#define TRUE 0
#define INIT_TOKEN_CAPACITY 16

static struct Token first_token(const char buffer[MAX_TOKEN_VALUE],
                                char current_char) {
    struct Token temp;

    if (strcmp(buffer, "int") == TRUE) {
        temp.type = TOKEN_INT;
    } else if (strcmp(buffer, "fun") == TRUE) {
        temp.type = TOKEN_FUNCTION;
    } else if (strcmp(buffer, "char") == TRUE) {
        temp.type = TOKEN_CHAR;
    } else if (strcmp(buffer, "float") == TRUE) {
        temp.type = TOKEN_FLOAT;
    } else if (strcmp(buffer, "let") == TRUE) {
        temp.type = TOKEN_LET;
    } else if (strcmp(buffer, "void") == TRUE) {
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

    strncpy(temp.value, buffer, sizeof(temp.value));
    return temp;
}

static void push_token(struct Token** tokens, const struct Token* token,
                       int* tokens_capacity, int* tokens_count) {
    if (*tokens_count >= *tokens_capacity) {
        *tokens_capacity *= 2;
        struct Token* new_tokens =
            realloc(*tokens, sizeof(struct Token) * (*tokens_capacity));
        if (!new_tokens) {
            perror("[-] Can't allocate new space \n");
            exit(1);
        }
        *tokens = new_tokens;
    }
    (*tokens)[*tokens_count] = *token;
    (*tokens_count)++;
}

struct Token* tokenizer(FILE* file) {
    char line[MAX_LINE_LEN];
    unsigned int column = 0;

    int tokens_capacity = INIT_TOKEN_CAPACITY;
    int tokens_count = 0;
    struct Token* tokens = malloc(sizeof(struct Token) * tokens_capacity);
    if (tokens == NULL) {
        fprintf(stderr, "[-] Can't allocate memory for token");
        exit(1);
    }

    char buffer[MAX_TOKEN_VALUE];
    int count = 0;
    buffer[0] = '\0';

    while (fgets(line, sizeof(line), file)) {
        column++;
        size_t line_lenght = strlen(line);

        for (int line_position = 0; line_position < line_lenght;
             line_position++) {
            char current_char = line[line_position];

            // TEXT
            if (current_char == '"') {
                struct Token temp = {0};
                temp.type = TOKEN_TEXT;
                int ti = 0;
                unsigned int start_col = column;

                for (line_position++;
                     line_position < line_lenght && line[line_position] != '"';
                     line_position++) {
                    if (ti < (int)sizeof(temp.value) - 1)
                        temp.value[ti++] = line[line_position];
                }
                temp.value[ti] = '\0';
                temp.column = start_col;
                strncpy(temp.full_line, line, sizeof(temp.full_line));

                push_token(&tokens, &temp, &tokens_capacity, &tokens_count);
                continue;
            }

            // LETTER
            if (current_char == '\'') {
                struct Token temp = {0};
                temp.type = TOKEN_LETTER;
                int ti = 0;
                unsigned int start_col = column;

                for (line_position++;
                     line_position < line_lenght && line[line_position] != '\'';
                     line_position++) {
                    if (ti < (int)sizeof(temp.value) - 1)
                        temp.value[ti++] = line[line_position];
                }
                temp.value[ti] = '\0';
                temp.column = start_col;
                strncpy(temp.full_line, line, sizeof(temp.full_line));

                push_token(&tokens, &temp, &tokens_capacity, &tokens_count);
                continue;
            }

            // Whitespace
            if (isspace(current_char)) {
                if (count > 0) {
                    buffer[count] = '\0';
                    struct Token temp = first_token(buffer, current_char);
                    temp.column = column;
                    strncpy(temp.full_line, line, sizeof(temp.full_line));
                    push_token(&tokens, &temp, &tokens_capacity, &tokens_count);
                    count = 0;
                    buffer[0] = '\0';
                }
                continue;
            }

            // Identifiers
            if (isalpha(current_char) || current_char == '_') {
                if (count < sizeof(buffer) - 1) {
                    buffer[count++] = (char)current_char;
                }
                continue;
            }

            // Digits
            if (isdigit(current_char) ||
                (current_char == '.' && isdigit(line[line_position + 1]))) {
                if (count < sizeof(buffer) - 1) {
                    buffer[count++] = (char)current_char;
                }
                continue;
            }

            // Symbols
            if (strchr("|=,;:{}()>&<!*.+-/", current_char)) {
                if (count > 0) {
                    buffer[count] = '\0';
                    struct Token temp = first_token(buffer, current_char);
                    temp.column = current_char;
                    strncpy(temp.full_line, line, sizeof(temp.full_line));
                    push_token(&tokens, &temp, &tokens_capacity, &tokens_count);
                    count = 0;
                    buffer[0] = '\0';
                }
                int next;
                struct Token temp = {0};
                switch (current_char) {
                    case '|':
                        next = fgetc(file);
                        if (next == '|') {
                            temp.type = TOKEN_OR;
                            strncpy(temp.value, "||", sizeof(temp.value));
                        } else {
                            temp.type = TOKEN_BITWISE_OR;
                            temp.value[0] = '|';
                            temp.value[1] = '\0';
                            ungetc(next, file);
                        }
                        break;
                    case ':':
                        temp.type = TOKEN_COLON;
                        temp.value[0] = ':';
                        temp.value[1] = '\0';
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
                            strncpy(temp.value, "++", sizeof(temp.value));
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
                            strncpy(temp.value, ">=", sizeof(temp.value));
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
                            strncpy(temp.value, "<=", sizeof(temp.value));
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
                            strncpy(temp.value, "==", sizeof(temp.value));
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
                            strncpy(temp.value, "!=", sizeof(temp.value));
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
                            strncpy(temp.value, "&&", sizeof(temp.value));
                        } else {
                            temp.type = TOKEN_BITWISE_AND;
                            temp.value[0] = '&';
                            temp.value[1] = '\0';
                            ungetc(next, file);
                        }
                        break;
                    default:
                        temp.type = TOKEN_IDENTIFIER;
                        temp.value[0] = (char)current_char;
                        temp.value[1] = '\0';
                        break;
                }

                temp.column = column;
                strncpy(temp.full_line, line, sizeof(temp.full_line));
                push_token(&tokens, &temp, &tokens_capacity, &tokens_count);
                continue;
            }

            fprintf(stderr, "[-] Invalid syntax %c \n", current_char);
            exit(1);
        }
    }

    // If buffer has leftover data at EOF
    if (count > 0) {
        buffer[count] = '\0';
        struct Token temp = first_token(buffer, '\0');
        temp.column = column;
        strncpy(temp.full_line, "end", sizeof(temp.full_line));
        push_token(&tokens, &temp, &tokens_capacity, &tokens_count);
        printf("[EOF-TOKEN] %s\n", temp.value);
    }

    // Add OEF token
    struct Token finish = {0};
    finish.type = TOKEN_OEF;
    strncpy(finish.value, "OEF", sizeof(finish.value));
    finish.column = column;
    strncpy(finish.full_line, "end", sizeof(finish.full_line));
    push_token(&tokens, &finish, &tokens_capacity, &tokens_count);

    return tokens;
}

