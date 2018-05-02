//
//  parser.h
//  SQLParser
//
//  Created by Anish Basu on 7/29/17.
//  Copyright © 2017 Anish Basu. All rights reserved.
//

#ifndef parser_h
#define parser_h

#include <stdio.h>
#include "lexer.h"
#include "database.h"

typedef struct _ArgNode {
    char* key;
    char* value;
    struct _ArgNode* next;
} ArgNode;

void print_args(ArgNode* args);
void parse_select(database** db, Token* root);
void parse_delete(database** db, Token* root);
void parse_insert(database** db, Token* root);
void parse_update(database** db, Token* root);
void parse_create(database** db, Token* root);
void parse_drop(database** db, Token* root);
void parse_query(database** db, Token* root);
void interactive(database** db, char* query);
ArgNode* add_arg(ArgNode** parent, char* key, char* value);
ArgNode* parse_k_till(Token** from, char* value);
ArgNode* parse_kv_till(Token** from, char* value);

#endif /* parser_h */
