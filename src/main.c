#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/tokenizer.h"
#include "../include/ast.h"

Node* parseNumber(Token* tokens, int* currentIndex){

}

Node* parseStatement(Token* tokens, int* currentIndex){

}



int main() {
  // -- Tokenizer --
  FILE *file = fopen("../test.cg", "r");

  if (!file) {
    perror("Errore apertura test.cg");
    return 1;
  }

  Token* tokens = tokenizer(file);
  fclose(file);

  printf("------ Instructions -------\n");

  // -- AST --
  
  // Make FunctionList
  FunctionList functionList = { .functions = NULL, .count = 0 };
  int currentIndex = 0;
  while(tokens[currentIndex].type != TOKEN_OEF){

    Token currentToken = tokens[currentIndex];
    if((currentToken.type == TOKEN_INT || currentToken.type == TOKEN_FLOAT || currentToken.type == TOKEN_CHAR || currentToken.type == TOKEN_VOID)  &&
        tokens[currentIndex+1].type == TOKEN_IDENTIFIER &&
        tokens[currentIndex+2].type == TOKEN_LPAREN ){
        Function* fun = malloc(sizeof(Function)); 
        *fun = parseFunction(tokens, &currentIndex);
        addFunctionToList(fun, &functionList);
    } else {
      currentIndex++;
    }

  }

  for(int i = 0; functionList.count > i; i++){
    Function* fun = functionList.functions[i];
    printf("[*] name: %s \n", fun->name);
  }

  return 0;
}
