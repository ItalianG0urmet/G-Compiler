#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/tokenizer.h"
#include "../include/ast.h"

int main(int argc, char* argv[]) {
  // -- Tokenizer --
  FILE *file = fopen(argv[1], "r");
  if (!file) {
    if(argv[1] == NULL) fprintf(stderr, "[-] You should specify an argument \n");
    else fprintf(stderr, "[-] Can't find %s \n", argv[1]);

    return 1;
  }
  Token* tokens = tokenizer(file);
  fclose(file);

  // -- AST --
  // Make FunctionList
  printf("------ Instructions -------\n");
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

  //Debug
  for(int i = 0; functionList.count > i; i++){
    Function* fun = functionList.functions[i];
    printf("[*] Name: %s \n", fun->name);
  }

  return 0;
}
