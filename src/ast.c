#include "../include/ast.h"

//Function getFunctionByName(char* name, FunctionList funList){
//  int index = 0;
//  while(index < funList.count){
//    if(strcmp(funList.functions[index]->name, name)){
//      return funList.functions[index];
//    }
//    index++;
//  }
//
//  return NULL;
//}

// Create function
Function parseFunction(Token* tokens, int* currentIndex){
  Token type = tokens[*currentIndex];
  Token identifier = tokens[*currentIndex+1];
  Argument* args = parseParam(tokens, *currentIndex, *currentIndex + 3);

  while(tokens[*currentIndex].type == TOKEN_LBRACE){
    (*currentIndex)++;
  }

  Function fun;
  fun.arguments = args; 

  if (identifier.type != TOKEN_IDENTIFIER || identifier.value == NULL) {
      printf("[-] Invalid function identifier\n");
      exit(1);
  }
  strncpy(fun.name, identifier.value, sizeof(fun.name)); 

  //TODO: NOW LETS DEFINE THE AALL
  


  (*currentIndex)++;
  return fun;
}

// Get args for function
Argument* parseParam(Token* tokens, int currentIndex, int firstParamIndex) {
  int index = firstParamIndex;
  int argCount = 0;

  while(tokens[index].type != TOKEN_RPAREN && tokens[index].type != TOKEN_OEF){
    if (tokens[index].type == TOKEN_INT ||
        tokens[index].type == TOKEN_FLOAT ||
        tokens[index].type == TOKEN_CHAR) {
      argCount++;
    }

    index += 2;
    if (tokens[index].type == TOKEN_COMMA) {
      index++;
    }
  }

  Argument* arguments = malloc(sizeof(Argument) * argCount);
  if (!arguments) {
    printf("[-] Can't allocate memory for args\n");
    exit(1);
  }

  index = firstParamIndex;
  int argIndex = 0;
  while(tokens[index].type != TOKEN_RPAREN && tokens[index].type != TOKEN_OEF){
    Argument arg;

    switch(tokens[index].type){
      case TOKEN_INT:   arg.type = ARG_INT; break;
      case TOKEN_FLOAT: arg.type = ARG_FLOAT; break;
      case TOKEN_CHAR:  arg.type = ARG_CHAR; break;
      default:
        printf("[-] Invalid param: %s\n", tokens[index].value);
        exit(1);
    }

    index += 2;
    arguments[argIndex++] = arg;

    if (tokens[index].type == TOKEN_COMMA) {
      index++;
    }
  }

  printf("[+] Found %d param\n", argCount);
  return arguments;
}


void addFunctionToList(Function* fun, FunctionList* functionList) {
  functionList->functions = realloc(functionList->functions, sizeof(Function*) * (functionList->count + 1));
  
  if (!functionList->functions) {
    printf("[-] Error reallocating memory for function list\n");
    exit(1);
  }

  functionList->functions[functionList->count] = fun;
  functionList->count++;
}
