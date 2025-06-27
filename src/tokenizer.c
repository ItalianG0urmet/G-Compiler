#include "../include/tokenizer.h"

Token firstToken(char buffer[256], char current) {

    Token temp;
    if (strcmp(buffer, "int") == 0) {
        temp.type = TOKEN_INT;
    } else if (strcmp(buffer, "float") == 0) {
        temp.type = TOKEN_FLOAT;
    } else if (strcmp(buffer, "void") == 0) {
        temp.type = TOKEN_VOID;
    } else {
        bool isNumeric = true;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (!isdigit(buffer[i])) {
                isNumeric = false;
                break;
            }
        }

        if (isNumeric) {
            temp.type = TOKEN_INT;
        } else {
            temp.type = TOKEN_IDENTIFIER;
        }
    }

    //TODO: Mettere un identifier a parte tra int e interi definit
    strcpy(temp.value, buffer);
    return temp;
}

void tokenPush(Token** tokens, Token* token, int* tokens_capacity, int *tokens_count){

  if(*tokens_count >= *tokens_capacity){
    *tokens_capacity *= 2;
    Token* new_tokens = realloc(*tokens, sizeof(Token) * (*tokens_capacity));
    if(!new_tokens){
      perror("Can't allocate new space \n");
      exit(1);
    }
    *tokens = new_tokens;
  } 
  (*tokens)[*tokens_count] = *token;
  (*tokens_count)++;
}
static const char* tokenTypeToString(TokenType type) {
    switch(type) {
        case TOKEN_TEXT: return "TEXT";
        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_CHAR: return "CHAR";
        case TOKEN_VOID: return "VOID";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_END: return "END";
        case TOKEN_OEF: return "OEF";
        default: return "UNKNOWN";
    }
}

Token* tokenizer(FILE* file){
    int tokens_capacity = 16;
    int tokens_count = 0;
    Token* tokens = malloc(sizeof(Token) * tokens_capacity);

    char buffer[256];
    int count = 0;
    buffer[0] = '\0';

    int current;
    while((current = fgetc(file)) != EOF){


        // -- TEXT  --
        if (current == '"') {
            Token temp = {0};
            temp.type = TOKEN_TEXT;
            int i = 0;

            current = fgetc(file);
            while (current != '"' && current != EOF && i < sizeof(temp.value) - 1) {
                temp.value[i++] = (char)current;
                current = fgetc(file);
            }
            temp.value[i] = '\0';

            tokenPush(&tokens, &temp, &tokens_capacity, &tokens_count);
            continue;
        }

        // -- Whitespace --
        if (isspace(current)) {
            if (count > 0) {
                buffer[count] = '\0';
                Token temp = firstToken(buffer, current);
                tokenPush(&tokens, &temp, &tokens_capacity, &tokens_count);
                count = 0;
                buffer[0] = '\0';
            }
            continue;
        }

        // -- Identifiers --
        if (isalpha(current) || current == '_') {
            if (count < sizeof(buffer) - 1) {
                buffer[count++] = (char)current;
            }
            continue;
        }

        // -- Digits --
        if (isdigit(current)) {
            if (count < sizeof(buffer) - 1) {
                buffer[count++] = (char)current;
            }
            continue;
        }

        // -- Symbols --
        if (strchr("=,;{}()", current)) {
            if (count > 0) {
                buffer[count] = '\0';
                Token temp = firstToken(buffer, current);
                tokenPush(&tokens, &temp, &tokens_capacity, &tokens_count);
                count = 0;
                buffer[0] = '\0';
            }

            Token temp = {0};
            switch (current) {
                case '=': temp.type = TOKEN_ASSIGN; break;
                case ';': temp.type = TOKEN_END;    break;
                case ',': temp.type = TOKEN_COMMA;  break;
                case '(': temp.type = TOKEN_LPAREN; break;
                case ')': temp.type = TOKEN_RPAREN; break;
                case '{': temp.type = TOKEN_LBRACE; break;
                case '}': temp.type = TOKEN_RBRACE; break;
                default:  temp.type = TOKEN_IDENTIFIER; break;
            }
            temp.value[0] = (char)current;
            temp.value[1] = '\0';

            tokenPush(&tokens, &temp, &tokens_capacity, &tokens_count);
            continue;
        }

    }

    // If buffer has leftover data at EOF
    if (count > 0) {
        buffer[count] = '\0';
        Token temp = firstToken(buffer, '\0');
        tokenPush(&tokens, &temp, &tokens_capacity, &tokens_count);
        printf("[EOF-TOKEN] %s [%s]\n", temp.value, tokenTypeToString(temp.type));
    }

    // Add OEF token
    Token finish = {0};
    finish.type = TOKEN_OEF;
    strcpy(finish.value, "404");
    tokenPush(&tokens, &finish, &tokens_capacity, &tokens_count);

    // Print all tokens
    printf("\n--- FINAL TOKENS ---\n");
    for (int i = 0; i < tokens_count; i++) {
        printf("[%02d] %s, %s\n", i, tokens[i].value, tokenTypeToString(tokens[i].type));
    }
    return tokens;
}



