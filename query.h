//
//  query.h
//  MiniDB
//
//  Created by Anish Basu on 7/30/17.
//  Copyright © 2017 Anish Basu. All rights reserved.
//

#ifndef query_h
#define query_h

#include <stdio.h>
#include "rownodes.h"
#include "parser.h"
#include "database.h"

typedef struct _QueryNode {
    rownode* data;
    struct _QueryNode* next;
} QueryNode;

QueryNode* add_queryNode(QueryNode** parent, rownode** child);

table* get_table(database** db, ArgNode* arg_table);

QueryNode* exc_select(database** db, ArgNode* table, ArgNode* fields, ArgNode* condition);
size_t exc_delete(database** db, ArgNode* table, ArgNode* condition);
void exc_insert(database** db, ArgNode* table, ArgNode* values);
size_t exc_update(database** db, ArgNode* table, ArgNode* updates, ArgNode* condition);
size_t exc_create(database** db, ArgNode* table, ArgNode* fields);
size_t exc_drop(database** db, ArgNode* table);

void print_queryNodes(QueryNode* root);
void print_tbl_headers(database** db, ArgNode* table);
#endif /* query_h */
