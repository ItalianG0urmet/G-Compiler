#include "../include/ast.h"

static inline void checkIfAllocated(const void* ptr){
    if(ptr == NULL){
        fprintf(stderr, "[-] Failed to allocate memory \n");
        exit(1);
    }
}

// -- Parse the symbols of and expression --
static Node* parseExpression(const Token* tokens, int* currentIndex) {
    // TODO: Create expression parser
    if (tokens[(*currentIndex + 1)].type != TOKEN_LPAREN) {

    }
}

static Node* parseIf(const Token* tokens, int* currentIndex);

// -- Transfrom some tokens into node --
static Node* transformIntoNode(const Token* tokens, int* currentIndex) {
    Node* node = calloc(1, sizeof(Node));
    checkIfAllocated(node);

    // If
    if (tokens[*currentIndex].type == TOKEN_IDENTIFIER &&
        strcmp(tokens[*currentIndex].value, "if") == 0) {
        printf("[+] Found if \n");
        parseIf(tokens, currentIndex);
    }

    // Int & not init
    if (tokens[*currentIndex].type == TOKEN_INT &&
        strcmp(tokens[*currentIndex].value, "int") == 0) {
        if (tokens[(*currentIndex) + 1].type == TOKEN_IDENTIFIER) {
            if (tokens[(*currentIndex) + 2].type == TOKEN_END) {
                strncpy(node->name, tokens[(*currentIndex) + 1].value,
                        sizeof(node->name));
                node->type = NO_ASSIGN_INT;
                printf("[+] Added not defined int named %s \n", node->name);
                (*currentIndex) += 3;
                return node;
            } else if (tokens[(*currentIndex) + 2].type == TOKEN_ASSIGN) {
                if (tokens[(*currentIndex) + 3].type == TOKEN_INT &&
                    strcmp(tokens[(*currentIndex) + 3].value, "int") != 0) {
                    strncpy(node->name, tokens[(*currentIndex) + 1].value,
                            sizeof(node->name));
                    node->type = NO_ASSIGN_INT;
                    printf("[+] Added defined int named %s \n", node->name);
                    (*currentIndex) += 5;
                    return node;
                } else {
                    fprintf(stderr, "[-] Syntax error \n");
                    exit(1);
                }
            } else {
                fprintf(stderr, "[-] Invalid var \n");
                exit(1);
            }
        }
    }

    // Return
    if (tokens[*currentIndex].type == TOKEN_IDENTIFIER &&
        strcmp(tokens[*currentIndex].value, "return") == 0) {
        if (tokens[(*currentIndex) + 1].type == TOKEN_INT &&
            strcmp(tokens[(*currentIndex) + 1].value, "int") != 0) {
            int returnCode = atoi(tokens[(*currentIndex) + 1].value);
            node->type = NO_RETURN;
            node->number = returnCode;
            printf("[+] Added return type, that return %d \n", returnCode);
            (*currentIndex) += 3;
            return node;
        } else {
            fprintf(stderr, "[-] Invalid syntax on return function \n");
            exit(1);
        }
    }

    // Text
    if (tokens[*currentIndex].type == TOKEN_TEXT &&
        tokens[(*currentIndex) - 1].type != TOKEN_ASSIGN) {
        if (tokens[(*currentIndex) + 1].type == TOKEN_END) {
            node->type = NO_PRINT;
            strncpy(node->name, tokens[*currentIndex].value,
                    sizeof(node->name));
            (*currentIndex) += 2;
            printf("[+] Added text type, that contains \"%s\" \n", node->name);
            return node;
        }
    }

    fprintf(
        stderr,
        "[-] Unknown or unsupported token at index %d (type=%d, value='%s')\n",
        *currentIndex, tokens[*currentIndex].type, tokens[*currentIndex].value);
    exit(1);
}

// -- Parse the if statment
static Node* parseIf(const Token* tokens, int* currentIndex) {
    Node* node = calloc(1, sizeof(Node));
    checkIfAllocated(node);

    node->type = TOKEN_IDENTIFIER;

    // Syntax check
    if (tokens[(*currentIndex + 1)].type != TOKEN_LPAREN) {
        fprintf(stderr,
                "[-] Wrong syntax expected '(' afet 'if' but found '%s' \n",
                tokens[(*currentIndex + 1)].value);
        exit(1);
    }

    *currentIndex += 2;
    node->lnode = parseExpression(tokens, currentIndex);

    if (tokens[*currentIndex].type != TOKEN_RPAREN) {
        fprintf(stderr,
                "[-] Wrong syntax expected ')' afet 'if' but found '%s' \n",
                tokens[*currentIndex].value);
        exit(1);
    }
    (*currentIndex)++;

    if (tokens[*currentIndex].type != TOKEN_LBRACE) {
        fprintf(stderr, "[-] Wrong syntax expected '{' afet 'if()' \n");
        exit(1);
    }
    (*currentIndex)++;

    // Generate all the nodes
    Node* thenBody = calloc(1, sizeof(Node));
    checkIfAllocated(thenBody);
    thenBody->type = NO_BODY;
    Node* thenLast = NULL;
    while (tokens[*currentIndex].type != TOKEN_RBRACE &&
           tokens[*currentIndex].type != TOKEN_OEF) {
        Node* stmt = transformIntoNode(tokens, currentIndex);
        if (!thenBody->body) {
            thenBody->body = stmt;
        } else {
            thenLast->next = stmt;
        }
        thenLast = stmt;
    }

    // Syntax check
    if (tokens[*currentIndex].type != TOKEN_RBRACE) {
        fprintf(stderr, "[-] Expected '}' to close if body\n");
        exit(1);
    }
    (*currentIndex)++;

    // Attach
    node->then = thenBody;
}

// -- Get a function by name --
Function* getFunctionByName(const char* name, const FunctionList funList) {
    for (int i = 0; i < funList.count; i++) {
        if (strcmp(funList.functions[i]->name, name) == 0) {
            return funList.functions[i];
        }
    }
    return NULL;
}

// -- Create a function --
Function parseFunction(const Token* tokens, int* currentIndex) {
    Token identifier = tokens[*currentIndex + 1];

    // Find return type
    FunReturnType returnType;
    switch (tokens[*currentIndex].type) {
        case TOKEN_VOID:
            returnType = RET_VOID;
            break;
        case TOKEN_INT:
            returnType = RET_INT;
            break;
        case TOKEN_CHAR:
            returnType = RET_CHAR;
            break;
        case TOKEN_FLOAT:
            returnType = RET_FLOAT;
            break;
        default:
            fprintf(stderr, "[-] Can't find return type of %s\n",
                    identifier.value);
            exit(1);
    }

    // Parse the args and define
    Argument* args = parseParam(tokens, *currentIndex, *currentIndex + 3);
    while (tokens[*currentIndex].type == TOKEN_LBRACE) {
        (*currentIndex)++;
    }

    Function fun;
    fun.arguments = args;
    fun.returnType = returnType;

    if (identifier.type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "[-] Invalid function identifier\n");
        exit(1);
    }
    strncpy(fun.name, identifier.value, sizeof(fun.name));

    // Skip to {
    while (tokens[*currentIndex].type != TOKEN_LBRACE) {
        (*currentIndex)++;
    }
    (*currentIndex)++;

    // Start node transformation
    Node* body = calloc(1, sizeof(Node));
    checkIfAllocated(body);
    body->type = NO_BODY;
    Node* last = NULL;
    int nodeCount = 0;
    printf("\n[+] Defining function %s [+]\n", fun.name);
    while (tokens[*currentIndex].type != TOKEN_RBRACE &&
           tokens[*currentIndex].type != TOKEN_OEF) {
        Node* node = transformIntoNode(tokens, currentIndex);

        if (node == NULL) {
            fprintf(stderr, "[-] Failed to create AST node at token index %d\n",
                    *currentIndex);
            exit(1);
        }

        if (body->body == NULL) {
            body->body = node;
        } else {
            last->next = node;
        }

        last = node;
        nodeCount++;
    }

    if (last) {
        last->next = NULL;
    }

    fun.body = body;
    fun.nodeCount = nodeCount;
    printf("[*] %s function have %d nodes \n", fun.name, fun.nodeCount);
    (*currentIndex)++;
    return fun;
}

// Get args for function
Argument* parseParam(const Token* tokens, int currentIndex, const int firstParamIndex) {
    int index = firstParamIndex;
    int argCount = 0;

    while (tokens[index].type != TOKEN_RPAREN &&
           tokens[index].type != TOKEN_OEF) {
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

    Argument* arguments = calloc(argCount, sizeof(Argument));
    checkIfAllocated(arguments);

    index = firstParamIndex;
    int argIndex = 0;
    while (tokens[index].type != TOKEN_RPAREN &&
           tokens[index].type != TOKEN_OEF) {
        Argument arg;

        switch (tokens[index].type) {
            case TOKEN_INT:
                arg.type = ARG_INT;
                break;
            case TOKEN_FLOAT:
                arg.type = ARG_FLOAT;
                break;
            case TOKEN_CHAR:
                arg.type = ARG_CHAR;
                break;
            default:
                fprintf(stderr, "[-] Invalid param: %s\n", tokens[index].value);
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
    functionList->functions = realloc(
        functionList->functions, sizeof(Function*) * (functionList->count + 1));

    if (!functionList->functions) {
        fprintf(stderr, "[-] Error reallocating memory for function list\n");
        exit(1);
    }

    functionList->functions[functionList->count] = fun;
    functionList->count++;
}
