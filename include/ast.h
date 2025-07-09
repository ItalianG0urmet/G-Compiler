#ifndef AST_H
#define AST_H

#include "tokenizer.h"

typedef enum {
    ARG_INT,
    ARG_FLOAT,
    ARG_CHAR,
} ArgumentType;

typedef struct {
    ArgumentType type;
    union {
        int   i;
        float f;
        char  str;
    };
} Argument;

typedef enum {
    NO_BODY,

    NO_ADD,
    NO_SUB,
    NO_MUL,
    NO_DIV,

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
} NodeType;

typedef struct Node Node;

struct Node {
    NodeType type;
    Node* lnode;
    Node* rnode;

    Node* code;  // IF, FOR
    Node* then;  // IF or FOR body
    Node* els;   // ELSE
    Node* init;  // INIT FOR
    Node* inc;   // INCREMENT FOR

    Node* body;  // Point to the first node
    Node* next;  // Point to the next node

    char name[256];  // Name of var or Name of text
    union{
        char letter;     // Just for chars
        int  number;     // only for NO_NUM or NO_RETURN
        float floating; // only for NO_NUM or NO_RETURN
    };
};

typedef enum { RET_INT, RET_CHAR, RET_FLOAT, RET_VOID } FunReturnType;

typedef struct {
    FunReturnType returnType;
    Node*         body;
    int           nodeCount;
    Argument*     arguments;
    char          name[256];
} Function;

typedef struct {
    Function** functions;
    int        count;
} FunctionList;

Function* getFunctionByName(const char* name, const FunctionList funList);
Argument* parseParam(const Token* tokens, int currentIndex, const int firstParamIndex);
Function parseFunction(const Token* tokens, int* currentIndex);
void addFunctionToList(Function* fun, FunctionList* functionList);

#endif
