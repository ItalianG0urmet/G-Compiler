#ifndef AST_H
#define AST_H

#include <tokenizer.h>

typedef enum {
  ARG_INT,
  ARG_FLOAT,
  ARG_CHAR,
} ArgumentType;

typedef struct {
  ArgumentType type;
  union {
    int i;
    float f;
    char str;
  };
   
} Argument;

typedef enum {
  NO_ADD,
  NO_SUB,
  NO_MUL,
  NO_DIV,
  NO_ASSIGN,
  NO_NUM,
  NO_CHAR,
  NO_RETURN,
  NO_IF,
  NO_FOR,
  NO_FUNCALL
} NodeType;

typedef struct Node Node;

struct Node {
  NodeType type;
  Node* lnode;
  Node* rnode;

  Node* code; // IF, FOR
  Node* then; // IF or FOR body
  Node* els; // ELSE
  Node* init; // INIT FOR
  Node* inc; // INCREMENT FOR

  Node* body;
  Node* next;

  int number; // only for NO_NUM
  char str; // only for NO_CHAR
};

typedef struct {
  Node** nodes;
  Argument* arguments;
  char name[256];
} Function;

typedef struct {
  Function** functions;
  int count;
} FunctionList;

Node* getFunctionByName(char* name, FunctionList functionList);
Argument* parseParam(Token* tokens, int currentIndex, int firstParamIndex);
Function parseFunction(Token* tokens, int* currentIndex);
void addFunctionToList(Function* fun, FunctionList* functionList);

#endif
