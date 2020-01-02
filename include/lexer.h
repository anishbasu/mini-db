//
//  lexer.h
//  SQLParser
//
//  Created by Anish Basu on 7/29/17.
//  Copyright © 2017 Anish Basu. All rights reserved.
//

#ifndef lexer_h
#define lexer_h


#ifdef _WINDOWS
#define strcasecmp stricmp
#endif

#include <stdio.h>

typedef enum { KEYWORD, IDENTIFIER, VALUE, OPERATOR} TokenType;

typedef struct _Token{
    char* value;
    TokenType type;
    struct _Token* next;
} Token;



Token* create_token(char* value, TokenType type);
void add_token(Token** parent, Token** child);
int is_any(char c, char* in_str);
off_t read_string(char* buf, char** payload, int* val_string);
int is_keyword(char* str);
int is_number(char* str);
Token* tokenize(char* query);

#endif /* lexer_h */
