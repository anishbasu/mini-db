//
//  query.c
//  MiniDB
//
//  Created by Anish Basu on 7/30/17.
//  Copyright © 2017 Anish Basu. All rights reserved.
//

#include "query.h"
#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "rownodes.h"
#include "rows.h"
#include "generics.h"
#include "database.h"
#include "tables.h"

QueryNode* add_queryNode(QueryNode** parent, rownode** child){
    QueryNode* new_node = (QueryNode *) malloc(sizeof(QueryNode));
    new_node->data = *child;
    new_node->next = NULL;
    if(*parent == NULL){
        *parent = new_node;
    } else {
        (*parent)->next = new_node;
    }
    return new_node;
}

table* get_table(database** db, ArgNode* arg_table){
    if(arg_table != NULL){
        for(tablenode* it = (*db)->root; it != NULL; it = it->next){
            if(streq(it->name, arg_table->key) && it->table != NULL){
                
                return it->table;
            }
        }
    }
    return NULL;
}

int map_name_to_col(table** table, char* col){
    for(int i = 0; i < strlen((*table)->field_types); i++){
        if(streq((*table)->field_names[i], col)){
            return i;
        }
    }
    return -1;
}



QueryNode* exc_select(database** db, ArgNode* arg_table, ArgNode* fields, ArgNode* condition){
    table* in_table = get_table(db, arg_table);
    QueryNode* results = NULL;
    QueryNode* last = NULL;
    //Set up conditions
    int col = map_name_to_col(&in_table, condition->key);
    i_type type = get_type(in_table->field_types[col]);
    generic* cond = (generic *) malloc(sizeof(generic));
    create_generic_str(&cond, type, condition->value);
    if(in_table == NULL){
        return NULL;
    }
    for(rownode* it = in_table->root; it != NULL; it = it->next){
        if(it->data != NULL){ //Ignore deletes
            if(geneq(it->data->columns[col], cond)){
                if(last == NULL) {
                    last = add_queryNode(&results, &it);
                } else {
                    last = add_queryNode(&last, &it);
                }
            }
        }
    }
    
    return results;
}

size_t create_cols(ArgNode* fields, char*** field_names, char** out_field_types){
    char* field_types = (char*) malloc(255);
    off_t pos = 0;
    for(ArgNode* it = fields; it != NULL; it = it->next){
        if(streq(it->value, "int")){
            field_types[pos] = 'i';
            pos++;
        }
        if(streq(it->value, "char")){
            field_types[pos] = 'c';
            pos++;
        }
        if(streq(it->value, "str")){
            field_types[pos] = 's';
            pos++;
        }
    }
    field_types[pos] = '\0';
    field_types = realloc(field_types, pos);
    *out_field_types = field_types;
    *field_names = (char **) malloc(sizeof(char*) * pos);
    pos = 0;
    for(ArgNode* it = fields; it != NULL; it = it->next){
        (*field_names)[pos] = it->key;
        pos++;
    }
    return pos;
}

size_t exc_create(database** db, ArgNode* arg_table, ArgNode* fields){
    char* table_name = arg_table->key;
    char** field_names;
    char* field_types;
    create_cols(fields, &field_names, &field_types);
    table* new_table = create_table(table_name, &field_names, &field_types);
    db_insert_table(db, &new_table);
    return 1;
}

size_t exc_update(database** db, ArgNode* arg_table, ArgNode* updates, ArgNode* condition){
    table* in_table = get_table(db, arg_table);
    if (in_table == NULL) {
        printf("Table doesn't exist");
        return 0;
    }
    QueryNode* results = exc_select(db, arg_table, NULL, condition);
    size_t pos = 0;
    for(QueryNode* it = results; it != NULL; it = it->next){
        for(ArgNode* arg = updates; arg != NULL; arg = arg->next){
            int col = map_name_to_col(&in_table, arg->key);
            switch(r_get_type(&it->data->data, col)){
                case int_t:
                    r_set_int(&it->data->data, col, atoi(arg->value));
                    break;
                case str_t:
                    r_set_str(&it->data->data, col, arg->value);
                    break;
                case char_t:
                    r_set_char(&it->data->data, col, arg->value[0]);
                    break;
                default:
                    break;
            }
        }
        pos++;
    }
    return pos;
}

size_t exc_delete(database** db, ArgNode* arg_table, ArgNode* condition){
    QueryNode* results = exc_select(db, arg_table, NULL, condition);
    size_t pos = 0;
    for(QueryNode* it = results; it != NULL; it = it->next){
        free(it->data->data);
        it->data->data = NULL;
        pos++;
    }
    return pos;
}

void exc_insert(database** db, ArgNode* arg_table, ArgNode* values){
    table* in_table = get_table(db, arg_table);
    if (in_table == NULL) {
        printf("Table doesn't exist");
    }
    if(in_table != NULL){
        row* new_row = create_row(in_table->field_types);
        for(ArgNode* arg = values; arg != NULL; arg = arg->next){
            int col = map_name_to_col(&in_table, arg->key);
            switch(r_get_type(&new_row, col)){
                case int_t:
                    r_set_int(&new_row, col, atoi(arg->value));
                    break;
                case str_t:
                    r_set_str(&new_row, col, arg->value);
                    break;
                case char_t:
                    r_set_char(&new_row, col, arg->value[0]);
                    break;
                default:
                    break;
            }
        }
        tbl_insert(&in_table, &new_row);
    }
}

size_t exc_drop(database** db, ArgNode* arg_table){
    if(arg_table != NULL){
        for(tablenode* it = (*db)->root; it != NULL; it = it->next){
            if(streq(it->name, arg_table->key)){
                free(it->table);
                it->table = NULL;
            }
        }
    }
    return 0;
}

void print_queryNodes(QueryNode* root){
    if(root != NULL){
        for(QueryNode* it = root; it != NULL; it = it->next){
            print_row(&it->data->data);
        }
    }
}

void print_tbl_headers(database** db, ArgNode* arg_table){
    table* in_table = get_table(db, arg_table);
    for(size_t i =0; i < strlen(in_table->field_types); i++){
        printf("%s\t", in_table->field_names[i]);
    }
    printf("\n");
}
