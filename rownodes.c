#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rownodes.h"

rownode* create_rownode(row** row_val){
	rownode* new_node = (rownode *) malloc(sizeof(rownode));
	new_node->data = *row_val;
	new_node->next = NULL;
	return new_node;
}

void add_node(rownode** parent_node, rownode** child_node){
	(*parent_node)->next = *child_node;
}

rownode* add_row(rownode** parent_node, row** new_row){
	rownode* new_node = create_rownode(new_row);
	add_node(parent_node, &new_node);
    return new_node;
}

void print_rownodes(rownode* root){
	rownode* it = root;
	while(it!=NULL){
		print_row(&it->data);
		it = it->next;
	}
}

size_t pack_rownodes(rownode* root, char** buf){
	//char* type_str = field_type_str(&root->data);
	//rownode* buf = create_row(&root->data->types);
	size_t row_sz = 0;
	size_t pos = 0;
	char* rownode_buf = malloc(sizeof(char));
	char* row_buf;
	rownode* it = root;
	while(it!=NULL){
        if(it->data !=NULL){
            row_sz = pack_row(&it->data, &row_buf);
            rownode_buf = (char *) realloc(rownode_buf, pos + row_sz + sizeof(size_t));
            memcpy(rownode_buf + pos, &row_sz, sizeof(size_t));
            pos+=sizeof(size_t);
            memcpy(rownode_buf + pos, row_buf, row_sz);
            pos+=row_sz;
            free(row_buf);
        }
		it = it-> next;
	}
	(*buf) = rownode_buf;
	return pos;
}

void unpack_rownodes(char* types, rownode** root, char** buf, size_t buf_sz){
	size_t pos = 0;
	size_t row_sz = 0;
	char* row_buf;
    *root = NULL;
	rownode* it;
	while(pos < buf_sz){
		memcpy(&row_sz, (*buf) + pos, sizeof(size_t));
		pos+= sizeof(size_t);
		row_buf = (char *) malloc(sizeof(char) * row_sz);
		memcpy(row_buf, (*buf) + pos, row_sz);
		pos+= row_sz;
		row* cur_row = unpack_row(types, &row_buf);
		free(row_buf);
		if(*root == NULL){
			*root = create_rownode(&cur_row);
			it = *root;
		} else {
			add_row(&it, &cur_row);
			it = it->next;
		}
	}
}
