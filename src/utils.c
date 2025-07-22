#include "../include/lexer_utils.h"

const char* token_type_to_string(const enum Token_type type) {
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



