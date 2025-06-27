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

static Node* transformIntoNode(Token* tokens, int* currentIndex){
  
  Node* node = malloc(sizeof(Node));
  if(!node){
    printf("[-] Failed token malloc");
    exit(1);
  }
  //Int not init
  if(tokens[*currentIndex].type == TOKEN_INT && strcmp(tokens[*currentIndex].value, "int") == 0){ 

    if(tokens[(*currentIndex) + 1].type == TOKEN_IDENTIFIER){

      if(tokens[(*currentIndex) + 2].type == TOKEN_END){
        strncpy(node->name, tokens[(*currentIndex) + 1].value, sizeof(node->name));
        node->type = NO_ASSIGN_INT;
        printf("[+] Not defined int: %s \n", node->name);
        (*currentIndex) += 3;
        return node;
      } else if(tokens[(*currentIndex) + 2].type == TOKEN_ASSIGN) {
        if (tokens[(*currentIndex) + 3].type == TOKEN_INT && strcmp(tokens[(*currentIndex) + 3].value, "int") != 0){
          strncpy(node->name, tokens[(*currentIndex) + 1].value, sizeof(node->name));
          node->type = NO_ASSIGN_INT;
          printf("[+] Defined int: %s \n", node->name);
          (*currentIndex) += 5;
          return node;
        } else {
          printf("[-] Syntax error \n");
          exit(1);
        }
      } else {
        printf("[-] Invalid var \n");
        exit(1);
      }

    }
  }

  //Return
  if(tokens[*currentIndex].type == TOKEN_IDENTIFIER && strcmp(tokens[*currentIndex].value, "return") == 0){
    if (tokens[(*currentIndex) + 1].type == TOKEN_INT && strcmp(tokens[(*currentIndex) + 1].value, "int") != 0){
      int returnCode = atoi(tokens[(*currentIndex) + 1].value);
      node->type = NO_RETURN;
      node->number = returnCode;
      printf("[*] Find return type, that return %d \n", returnCode);
      (*currentIndex) += 3;
      return node;
    } else {
      printf("[-] Invalid syntax on return function \n");
      exit(1);
    }
  }

  //Text
  if(tokens[*currentIndex].type == TOKEN_TEXT && tokens[(*currentIndex) - 1].type != TOKEN_ASSIGN){
    if(tokens[(*currentIndex) + 1].type == TOKEN_END){
      node->type = NO_PRINT;
      strncpy(node->name, tokens[*currentIndex].value, sizeof(node->name));
      (*currentIndex) += 2;
      printf("[*] Find text type, that contains \"%s\" \n", node->name);
      return node;   
    }
  }

  printf("[-] Unknown or unsupported token at index %d (type=%d, value='%s')\n", *currentIndex, tokens[*currentIndex].type, tokens[*currentIndex].value);
  (*currentIndex)++;
  return NULL;

}

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

  //Skip to {
  while(tokens[*currentIndex].type != TOKEN_LBRACE){
    (*currentIndex)++;
  }
  (*currentIndex)++;

  //Start node transformation
  Node* body = malloc(sizeof(Node));
  body->type = NO_BODY;
  body->next = NULL;
  body->body = NULL;
  Node* last = NULL;
  int nodeCount = 0;
  printf("\n[+] Definding function %s [+]\n", fun.name);
  while(tokens[*currentIndex].type != TOKEN_RBRACE && tokens[*currentIndex].type != TOKEN_OEF){
    Node* node = transformIntoNode(tokens, currentIndex);

    if (node == NULL) {
      printf("[-] Failed to create AST node at token index %d\n", *currentIndex);
      exit(1);
    } 

    if(body->body == NULL){
      body->body = node;
    } else {
      last->next = node;
    }

    last = node;
    nodeCount++;
  }
  fun.body = body;
  fun.nodeCount = nodeCount;
  printf("Per questa funzione abbiamo %d nodi \n", fun.nodeCount);
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
