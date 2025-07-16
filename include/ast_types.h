#ifndef AST_TYPES
#define AST_TYPES

#include "tokenizer.h"

typedef enum {
    ARG_INT,
    ARG_FLOAT,
    ARG_CHAR,
} argument_type_t;

typedef struct {
    argument_type_t type;
    union {
        int i;
        float f;
        char str;
    };
} argument_t;

typedef enum {
    NO_BODY,

    NO_ADD,
    NO_SUB,
    NO_MUL,
    NO_DIV,
    NO_INCREMENT,

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
} node_type_t;

typedef struct Node node_t;

struct Node {
    node_type_t type;
    node_t* lnode;
    node_t* rnode;

    node_t* code;  // IF, FOR
    node_t* then;  // IF or FOR body
    node_t* els;   // ELSE
    node_t* init;  // INIT FOR
    node_t* inc;   // INCREMENT FOR

    node_t* body;  // Point to the first node
    node_t* next;  // Point to the next node

    char name[256];  // Name of var or Name of text
    union {
        char letter;     // Just for chars
        int number;      // only for NO_NUM or NO_RETURN
        float floating;  // only for NO_NUM or NO_RETURN
    };
};

typedef enum { RET_INT, RET_CHAR, RET_FLOAT, RET_VOID } fun_return_type_t;

typedef struct {
    fun_return_type_t return_type;
    node_t* body;
    int node_count;
    argument_t* arguments;
    char name[256];
} function_t;

typedef struct {
    function_t** functions;
    int count;
} function_list_t;

#endif
