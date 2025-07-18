#include <llvm-c/BitWriter.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/ast.h"
#include "../include/llvm.h"
#include "../include/tokenizer.h"

int main(int argc, const char* argv[]) {
    // Tokenizer
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        if (argv[1] == NULL)
            fprintf(stderr, "[-] You should specify an argument \n");
        else
            fprintf(stderr, "[-] Can't find %s \n", argv[1]);

        return 1;
    }
    const token_t* tokens = tokenizer(file);
    fclose(file);

    // CREATE AST
    printf("------ Instructions -------");
    function_list_t function_list = {.functions = NULL, .count = 0};
    int current_index = 0;
    while (tokens[current_index].type != TOKEN_OEF) {
        const token_t current_token = tokens[current_index];
        if ((current_token.type == TOKEN_INT ||
             current_token.type == TOKEN_FLOAT ||
             current_token.type == TOKEN_CHAR ||
             current_token.type == TOKEN_VOID) &&
            tokens[current_index + 1].type == TOKEN_IDENTIFIER &&
            tokens[current_index + 2].type == TOKEN_LPAREN) {
            function_t* fun = malloc(sizeof(function_t));
            if (fun == NULL) {
                fprintf(stderr, "[-] Can't allocate memory \n");
                exit(1);
            }
            *fun = parse_function(tokens, &current_index);
            add_function_to_list(fun, &function_list);
        } else {
            current_index++;
        }
    }

    // GENERATE LLVM
    generate_llvm(function_list);

    return 0;
}
