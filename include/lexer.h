#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#define MAX_LINE_LEN 1024

enum Token_type {
    TOKEN_IDENTIFIER,  // identifier
    TOKEN_VOID,        // void
    TOKEN_INT,         // 0
    TOKEN_FLOAT,       // 0.0
    TOKEN_CHAR,        // char
    TOKEN_TEXT,        // "
    TOKEN_LETTER,      // '
    TOKEN_DOT,         // .

    TOKEN_NOT,     // !
    TOKEN_EQUAL,   // ==
    TOKEN_NEQUAL,  // !=
    TOKEN_EMAJOR,  // >=
    TOKEN_EMINOR,  // <=
    TOKEN_MAJOR,   // >
    TOKEN_MINOR,   // <
    TOKEN_OR,      // ||
    TOKEN_AND,     // &&

    TOKEN_BITWISE_OR,   // |
    TOKEN_BITWISE_AND,  // &

    TOKEN_ASTERISK,  // *

    TOKEN_PLUS,      // +
    TOKEN_MINUS,     // -
    TOKEN_DIVISION,  // /

    TOKEN_INCREMENT,  // ++

    TOKEN_ASSIGN,  // =
    TOKEN_LPAREN,  // (
    TOKEN_RPAREN,  // )
    TOKEN_RBRACE,  // }
    TOKEN_LBRACE,  // {
    TOKEN_COMMA,   // ,
    TOKEN_END,     // ;

    TOKEN_OEF,
};

struct Token {
    enum Token_type type;
    char value[256];
    int column;
    char full_line[MAX_LINE_LEN];
};

struct Token* tokenizer(FILE* file);

#endif
