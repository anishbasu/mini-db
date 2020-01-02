//
//  parser.c
//  SQLParser
//
//  Created by Anish Basu on 7/29/17.
//  Copyright © 2017 Anish Basu. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util.h"
#include "query.h"
#include "parser.h"

void print_args(ArgNode* args){
    for(ArgNode* it = args; it != NULL; it= it->next){
        printf("%s: %s\n", it->key, it->value);
    }
}

void interactive(database** db, char* query){
    Token* query_tok = tokenize(query);
    parse_query(db, query_tok);
}

void parse_select(database** db, Token* root){
    ArgNode* fields = parse_k_till(&root, "from");
    ArgNode* table = parse_k_till(&root, "where");
    ArgNode* conditions = parse_kv_till(&root, "\\EOF\\");
    if(table != NULL && fields != NULL && conditions != NULL){
        QueryNode* results = exc_select(db, table, fields, conditions);
        if(results != NULL){
            print_tbl_headers(db, table);
            print_queryNodes(results);
        } else {
            printf("------");
        }
        
    }
}

void parse_delete(database** db, Token* root){
    Token* expect_from = root->next;
    ArgNode* table = NULL;
    ArgNode* conditions = NULL;
    if(streq(expect_from->value, "from")){
        table = parse_k_till(&expect_from, "where");
        conditions = parse_kv_till(&expect_from, "\\EOF\\");
    }
    if(table != NULL && conditions != NULL){
        exc_delete(db, table, conditions);
    }
}

void parse_insert(database** db, Token* root){
    Token* iter = root->next;
    ArgNode* table = parse_k_till(&iter, "(");
    ArgNode* values = parse_kv_till(&iter, ")");
    if(table != NULL && values != NULL){
        exc_insert(db, table, values);
    }
}

void parse_update(database** db, Token* root){
    ArgNode* table = parse_k_till(&root, "set");
    ArgNode* updates = parse_kv_till(&root, "where");
    ArgNode* conditions = parse_kv_till(&root, "\\EOF\\");
    if(table != NULL && updates != NULL && conditions != NULL){
        exc_update(db, table, updates, conditions);
    }
}

void parse_create(database** db, Token* root){
    ArgNode* table = parse_k_till(&root, "fields");
    ArgNode* fields = parse_kv_till(&root, "\\EOF\\");
    if(table != NULL && fields != NULL){
        exc_create(db, table, fields);
    }
}

void parse_drop(database** db, Token* root){
    ArgNode* table = parse_k_till(&root, "\\EOF\\");
    if(table != NULL){
        exc_drop(db, table);
    }
}

void parse_query(database** db, Token* root){
    if(root->type == KEYWORD){
        if(streq(root->value, "select")){
            parse_select(db, root);
        }
        if(streq(root->value, "delete")){
            return parse_delete(db, root);
        }
        if(streq(root->value, "update")){
            return parse_update(db, root);
        }
        if(streq(root->value, "insert")){
            return parse_insert(db, root);
        }
        if(streq(root->value, "create")){
            return parse_create(db, root);
        }
        if(streq(root->value, "drop")){
            return parse_drop(db, root);
        }
    }
}


ArgNode* add_arg(ArgNode** parent, char* key, char* value){
    ArgNode* newArg = (ArgNode*) malloc(sizeof(ArgNode));
    mstrcpy(&newArg->key, &key);
    mstrcpy(&newArg->value, &value);
    if(*parent == NULL){
        *parent = newArg;
    } else {
        (*parent)->next = newArg;
    }
    newArg->next = NULL;
    return newArg;
}

ArgNode* parse_k_till(Token** from, char* value){
    ArgNode* keys = NULL;
    ArgNode* end = NULL;
    Token* it = *from;
    for(; it != NULL && !streq(it->value, value); it = it->next){
        if(it->type == IDENTIFIER){
            if(keys == NULL){
                end = add_arg(&keys, it->value, "");
            } else {
                end = add_arg(&end, it->value, "");
            }
        }
    }
    *from = it;
    return keys;
}

ArgNode* parse_kv_till(Token** from, char* value){
    ArgNode* keys = NULL;
    ArgNode* end = NULL;
    Token* it = *from;
    for(; it != NULL && !streq(it->value, value); it = it->next){
        if(it->type == IDENTIFIER){
            char* key = it->value;
            Token* op = it->next;
            
            if(op != NULL && op->type == OPERATOR && streq(op->value, "=")){
                Token* tk_value = op->next;
                if(tk_value->type == VALUE){
                    it = tk_value; //Skip to the value pointer
                    if(keys == NULL){
                        end = add_arg(&keys, key, tk_value->value);
                    } else {
                        end = add_arg(&end,  key, tk_value->value);
                    }
                }
                if(tk_value->type == KEYWORD){
                    it = tk_value;
                    if(streq(tk_value->value, "int") ||
                       streq(tk_value->value, "char")||
                       streq(tk_value->value, "str")){
                        if(keys == NULL){
                            end = add_arg(&keys, key, tk_value->value);
                        } else {
                            end = add_arg(&end,  key, tk_value->value);
                        }
                    }
                }
            }
        }
    }
    *from = it;
    return keys;
}
