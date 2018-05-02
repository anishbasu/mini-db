#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "rows.h"
#include "util.h"
size_t rowlen(row** in_row){
	return strlen((*in_row)->types);
}

row* create_row(char* types){
	if(verify_types(types)){
		row* new_row = (row *) malloc(sizeof(row));
		new_row->columns = (generic **) malloc(sizeof(generic *) * strlen(types));
		new_row->types = types;
		for(int i = 0; i < rowlen(&new_row); i++){
			create_def_generic(&new_row->columns[i], get_type(new_row->types[i]));
		}
		return new_row;
	} else {
		return NULL;
	}
}

i_type r_get_type(row** in_row, int index){
	return (*in_row)->columns[index]->type;
}

int r_get_int(row** in_row, int index){
	return get_int(&(*in_row)->columns[index]);
}

void r_set_int(row** in_row, int index, int value){
	set_int(&(*in_row)->columns[index], &value);
}

char r_get_char(row** in_row, int index){
	return get_char(&(*in_row)->columns[index]);
}

void r_set_char(row** in_row, int index, char value){
	set_int(&(*in_row)->columns[index], &value);
}

char* r_get_str(row** in_row, int index){
	return get_str(&(*in_row)->columns[index]);
}

void r_set_str(row** in_row, int index, char* value){
	set_str(&(*in_row)->columns[index], value);
}

void print_row(row** in_row){
	for(int i = 0; i < rowlen(in_row); i++){
		switch(r_get_type(in_row, i)){
			case int_t:
				printf("%i\t", r_get_int(in_row, i));
				break;
			case char_t:
				printf("%c\t", r_get_char(in_row, i));
				break;
			case str_t:
				printf("%s\t", r_get_str(in_row, i));
				break;
			default:
				printf("NULL\t");
				break;
		}
	}
	printf("\n");
}

size_t get_size(row** in_row){
	size_t rowsize = 0;
	for(int i = 0; i < rowlen(in_row); i++){
		switch(r_get_type(in_row, i)){
			case int_t:
				rowsize += sizeof(int);
				break;
			case char_t:
				rowsize += sizeof(char);
				break;
			case str_t:
				rowsize += sizeof(size_t) + sizeof(char) * (strlen(r_get_str(in_row, i)) + 1);
				break;
			default:
				break;
		}
	}
	return rowsize;
}

size_t pack_row(row** in_row, char** buf){
	*buf = malloc(get_size(in_row));
	size_t pos = 0;
	int value_i;
	char value_c;
	char* value_s;
	for(int i = 0; i < rowlen(in_row); i++){
		switch(r_get_type(in_row, i)){
			case int_t:
				value_i = r_get_int(in_row, i);
				memcpy(*buf + pos, &value_i, sizeof(int));
				pos+= sizeof(int);
				break;
			case char_t:
				value_c = r_get_char(in_row, i);
				memcpy(*buf + pos, &value_c, sizeof(char));
				pos+= sizeof(char);
				break;
			case str_t:
				value_s = r_get_str(in_row, i);
				pos+= pack_string(*buf + pos, value_s);
                free(value_s);
				break;
			default:
				break;
		}
	}
	return pos;
}

row* unpack_row(char* types, char** buf){
	row* new_row = create_row(types);
	size_t pos = 0;
	int value_i;
	char value_c;
	size_t len;
	char* value_s;
	for (int i = 0; i < rowlen(&new_row); i++)
	{
		switch(r_get_type(&new_row, i)){
			case int_t:
				value_i = 0;
				memcpy(&value_i, *buf + pos, sizeof(int));
				r_set_int(&new_row, i, value_i);
				pos+= sizeof(int);
				break;
			case char_t:
				value_c = 'a';
				memcpy(&value_c, *buf + pos, sizeof(char));
				r_set_char(&new_row, i, value_c);
				pos+= sizeof(char);
				break;
			case str_t:
				memcpy(&len, *buf + pos, sizeof(size_t));
				pos+= sizeof(size_t);
				value_s = (char *) malloc(len);
				memcpy(value_s, *buf + pos, len);
				r_set_str(&new_row, i, value_s);
				free(value_s);
				pos+= len;
				break;
			default:
				break;
		}
	}

	return new_row;
}
