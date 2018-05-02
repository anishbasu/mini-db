#ifndef ROWNODES
#define ROWNODES
#include "rows.h"

typedef struct _rownode{
	row* data;
	struct _rownode* next;
} rownode;

//Constructor
rownode* create_rownode(row** row_val);

//Add Node/Row
void add_node(rownode** parent_node, rownode** child_node);
rownode* add_row(rownode** parent_node, row** new_row);

//Print All Rows in Row Nodes
void print_rownodes(rownode* root);

//Binary Pack/Unpack Data
size_t pack_rownodes(rownode* root, char** buf);
void unpack_rownodes(char* types, rownode** root, char** buf, size_t buf_sz);
#endif
