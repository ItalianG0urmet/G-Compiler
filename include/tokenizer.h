#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    TOKEN_IDENTIFIER,   // identifier
    TOKEN_VOID,         // void
    TOKEN_INT,          // 0
    TOKEN_FLOAT,        // 0.0
    TOKEN_CHAR,         // char
    TOKEN_TEXT,         // "

    TOKEN_NOT,          // !
    TOKEN_EQUAL,        // ==
    TOKEN_NEQUAL,       // !=
    TOKEN_EMAJOR,       // >=
    TOKEN_EMINOR,       // <=
    TOKEN_MAJOR,        // >
    TOKEN_MINOR,        // <
    TOKEN_OR,           // ||
    TOKEN_AND,          // &&

    TOKEN_BITWISE_OR,   // |
    TOKEN_BITWISE_AND,  // &
    TOKEN_POINTER,      // *

    TOKEN_ASSIGN,       // =
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_RBRACE,       // }
    TOKEN_LBRACE,       // {
    TOKEN_COMMA,        // ,
    TOKEN_END,          // ;

    TOKEN_OEF,
} TokenType;

typedef struct {
    TokenType type;
    char      value[256];
} Token;

Token* tokenizer(FILE* file);

#endif
