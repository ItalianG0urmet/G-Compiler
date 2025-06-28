#include <stdio.h>
#include <llvm-c/Core.h>
#include <string.h>
#include <stdlib.h>
#include "../include/tokenizer.h"
#include "../include/ast.h"

void generateFunctionIR(Function* fun, LLVMContextRef* context, LLVMModuleRef* module){

  LLVMTypeRef voidType = LLVMVoidType();
  LLVMTypeRef intType = LLVMInt32Type();
  LLVMTypeRef charType = LLVMInt8Type();
  LLVMTypeRef floatType = LLVMFloatType();

  LLVMTypeRef returnType;
  switch(fun->returnType){
    case RET_VOID: returnType = voidType; break;
    case RET_INT: returnType = intType; break;
    case RET_CHAR: returnType = charType; break;
    case RET_FLOAT: returnType = floatType; break;
    default: fprintf(stderr, "[-] Can't find return type in IR \n");
  }

  Node* node = fun->body->body;
  while(node != NULL){
    printf("[DBG] node@%p next=%p type=%d\n", (void*)node, (void*)node->next, node->type);
    switch(node->type){
      case NO_ASSIGN_INT:
        printf("[P] Int\n");
        break;
      case NO_PRINT:
        printf("[P] Print\n");
        break;
      case NO_RETURN:
        printf("[P] Return\n");
        break;
      default:
        printf("[P] None\n");
        break;
    }
    node = node->next;
  }
}

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

  //Init LLVM IR convertion
  LLVMContextRef context = LLVMContextCreate();
  LLVMModuleRef module = LLVMModuleCreateWithName("module");
  Function* mainFun = getFunctionByName("main", functionList);
  if(mainFun == NULL){
    fprintf(stderr, "[-] Main entry point don't exist \n");
    exit(1);
  }
  generateFunctionIR(mainFun, &context, &module);

  return 0;
}
