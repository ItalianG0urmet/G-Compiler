#ifndef ERROR_UTILS_H
#define ERROR_UTILS_H

struct Token;  // form lexer.h

void send_syntax_error(const struct Token* token, const char* error);
void send_syntax_error_by_line(const char* line, const int column,
                                      const char* error);
#endif
