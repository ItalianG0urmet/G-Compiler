#include "../include/compiler.h"

__attribute__((noreturn, cold)) static void file_not_found_error(
    const char* file_path) {
    if (file_path == NULL)
        fprintf(stderr, "[-] You should specify an argument \n");
    else
        fprintf(stderr, "[-] Can't find the file %s \n", file_path);
    exit(1);
}

__attribute__((noreturn, cold)) static void memory_allocation_error() {
    fprintf(stderr, "[-] Can't allocate memory \n");
    exit(1);
}

static struct Token* get_tokens(const char* file_path, bool debug_active) {
    FILE* file = fopen(file_path, "r");
    if (!file) file_not_found_error(file_path);
    struct Token* tokens = tokenizer(file);
    fclose(file);

    if (debug_active) {
        printf("\n=========================");
        printf("\nGenerated tokens:\n");
        for (int i = 0; tokens[i].type != TOKEN_OEF; i++) {
            printf("  [%02d] %s, %s\n", i, tokens[i].value,
                   token_type_to_string(tokens[i].type));
        }
        printf("=========================\n\n");
    }

    return tokens;
}

static struct Function_list parse_functions(const struct Token* tokens) {
    struct Function_list function_list = {.functions = NULL, .count = 0};
    int current_index = 0;
    while (tokens[current_index].type != TOKEN_OEF) {
        const struct Token current_token = tokens[current_index];
        if ((current_token.type == TOKEN_INT ||
             current_token.type == TOKEN_FLOAT ||
             current_token.type == TOKEN_CHAR ||
             current_token.type == TOKEN_VOID) &&
            tokens[current_index + 1].type == TOKEN_IDENTIFIER &&
            tokens[current_index + 2].type == TOKEN_LPAREN) {
            struct Function* fun = calloc(1, sizeof(struct Function));
            if (fun == NULL) memory_allocation_error();
            *fun = parse_function(tokens, &current_index);
            add_function_to_list(fun, &function_list);
        } else {
            current_index++;
        }
    }

    return function_list;
}

void compile_file(const char* file_path, const char* file_output, bool debug_active) {
    printf("[+] Generating tokens... \n");
    const struct Token* tokens = get_tokens(file_path, debug_active);
    printf("[+] Parsing tokens... \n");
    const struct Function_list function_list = parse_functions(tokens);
    printf("[+] Generating llvm... \n");
    generate_llvm(function_list, file_output, debug_active);
}
