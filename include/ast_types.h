#ifndef AST_TYPES
#define AST_TYPES

#include <stdbool.h>

#define MAX_LINE_LEN 1024
#define MAX_IDENTIFIER_LENGTH 256

enum Argument_type {
    ARG_INT,
    ARG_FLOAT,
    ARG_CHAR,
};

struct Argument {
    enum Argument_type type;
    union {
        int i;
        float f;
        char str;
    };
};

enum Node_type {
    NO_BODY,

    NO_ADD,
    NO_SUB,
    NO_MUL,
    NO_DIV,
    NO_INCREMENT,

    NO_REASSIGN_INT,
    NO_REASSIGN_FLOAT,
    NO_REASSIGN_CHAR,

    NO_ASSIGN_INT,
    NO_ASSIGN_FLOAT,
    NO_ASSIGN_CHAR,

    NO_PRINT,
    NO_RETURN,
    NO_ASM,

    NO_IF,
    NO_MAJOR,
    NO_MINOR,
    NO_EQUAL_TO,

    NO_WHILE,
    NO_FOR,
    NO_FUNCALL
};

struct Node {
    enum Node_type type;
    struct Node* lnode;
    struct Node* rnode;

    struct Node* code;  // IF, FOR
    struct Node* then;  // IF or FOR body
    struct Node* els;   // ELSE
    struct Node* init;  // INIT FOR
    struct Node* inc;   // INCREMENT FOR

    struct Node* body;  // Point to the first node
    struct Node* next;  // Point to the next node

    char name[MAX_IDENTIFIER_LENGTH];  // Name of var or Name of text
    bool use_identifier;
    union {
        char letter;     // Just for chars
        int number;      // only for NO_NUM or NO_RETURN
        float floating;  // only for NO_NUM or NO_RETURN
        char identifier[MAX_IDENTIFIER_LENGTH];
    };

    char full_line[MAX_LINE_LEN];
    int column;
};

enum Function_return_type { RET_INT, RET_CHAR, RET_FLOAT, RET_VOID };

struct Function {
    enum Function_return_type return_type;
    struct Node* body;
    int node_count;
    struct Argument* arguments;
    char name[MAX_IDENTIFIER_LENGTH];
};

struct Function_list {
    struct Function** functions;
    int count;
};

#endif
