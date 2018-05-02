//
//  lexer.c
//  SQLParser
//
//  Created by Anish Basu on 7/29/17.
//  Copyright © 2017 Anish Basu. All rights reserved.
//

#include "lexer.h"
#include <string.h>
#include <stdlib.h>

char* Keywords[] = {
    "select",
    "from",
    "where",
    "join",
    "on",
    "table",
    "create",
    "delete",
    "insert",
    "into",
    "update",
    "drop",
    "varchar",
    "int",
    "char",
    "str",
    "set",
    "drop",
    "table",
    "fields"};

char* Whitespace = "\t \n\r";
char* Operator = "(=,)";
char* Digits = "0123456789";

Token* create_token(char* value, TokenType type){
    Token* new_tok = (Token *) malloc(sizeof(Token));
    new_tok->value = malloc(strlen(value) * sizeof(char));
    strcpy(new_tok->value, value);
    new_tok->type = type;
    new_tok->next = NULL;
    return new_tok;
}

void add_token(Token** parent, Token** child){
    (*parent)->next = *child;
}

int is_any(char c, char* in_str){
    char* idx_ptr = strchr(in_str, c);
    if(idx_ptr){
        return 1;
    } else {
        return 0;
    }
}

off_t read_string(char* buf, char** payload, int* val_string){
    off_t pos = 0;
    if(buf[pos] == '\''){
        *val_string = 1;
    } else {
        *val_string = 0;
    }
    
    while(pos < strlen(buf)){
        pos++;
        if(*val_string){ //Value Strings Take eveything in quotes. Escape quote marks with backslash
            if(buf[pos] == '\'' && buf[pos - 1] != '\\'){
                pos++; //Include Closing Quote
                break;
            }
        } else {
            if(is_any(buf[pos], Whitespace)) break;
            if(is_any(buf[pos], Operator)) break;
        }
    }
    *payload = (char *) malloc(pos * sizeof(char) + 1);
    strncpy(*payload, buf, pos);
    return pos;
}



int is_keyword(char* str){
    for(int i = 0; i < sizeof(Keywords) / sizeof(char *); i++){
        if(strcasecmp(str, Keywords[i]) == 0){
            return i;
        }
    }
    return -1;
}

int is_number(char* str){
    if(strspn(str, Digits) == strlen(str)) {
        return 1;
    } else {
        return 0;
    }
}

Token* tokenize(char* query){
    Token* root = NULL;
    Token* it = NULL;
    int is_value;
    char* str_payload;
    for(off_t pos = 0; pos < strlen(query);){
        if(is_any(query[pos], Whitespace)) {
            pos++; //Ignore Whitespace
        } else {
            Token* new_tok;
            if(!is_any(*(query + pos), Operator)){
                pos+=read_string(query + pos, &str_payload, &is_value);
                if(!is_value){
                    int keyword = is_keyword(str_payload);
                    if(keyword != -1){
                        new_tok = create_token(Keywords[keyword], KEYWORD);
                    } else {
                        if(is_number(str_payload)){
                            new_tok = create_token(str_payload, VALUE);
                        } else {
                            new_tok = create_token(str_payload, IDENTIFIER);
                        }
                    }
                } else {
                    new_tok = create_token(str_payload, VALUE);
                }
                
            } else {
                str_payload = (char *) malloc(2 * sizeof(char));
                snprintf(str_payload, 2, "%c", *(query + pos));
                new_tok = create_token(str_payload, OPERATOR);
                pos++;
            }
            if(root == NULL){
                root = new_tok;
                it = new_tok;
            } else {
                add_token(&it, &new_tok);
                it = new_tok;
            }
        }
    }
    return root;
}
