//
//  database.h
//  MiniDB
//
//  Created by Anish Basu on 7/30/17.
//  Copyright © 2017 Anish Basu. All rights reserved.
//

#ifndef database_h
#define database_h

#include <stdio.h>
#include "tables.h"

typedef struct _tablenode{
    char* name;
    table* table;
    struct _tablenode* next;
} tablenode;

typedef struct _database{
    char* name;
    tablenode* root;
    tablenode* end;
} database;

database* create_database(char* name);
size_t pack_db(database** db, char** buf);
size_t unpack_db(database** db, char* buf);
tablenode* add_tablenode(tablenode** parent, table** table);
off_t write_tables(database** db, FILE* fp);
off_t read_tables(database** db, FILE*fp);
void read_rows(database** db, FILE* fp);
off_t write_rows(database** db, FILE* fp);
void write_files(database** db);
void read_files(database** db);
void db_insert_table(database** db, table** table);
#endif /* database_h */
