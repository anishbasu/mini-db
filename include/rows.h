#ifndef ROWS
#define ROWS
#include "generics.h"
typedef struct {
	char* types;
	generic** columns;
} row;

//Constructor
row* create_row(char* types);

//Get Row Length
size_t rowlen(row** in_row);

//Getters/Setters
//Type
i_type r_get_type(row** in_row, int index);
//Integer
int r_get_int(row** in_row, int index);
void r_set_int(row** in_row, int index, int value);
//Character
char r_get_char(row** in_row, int index);
void r_set_char(row** in_row, int index, char value);
//String
char* r_get_str(row** in_row, int index);
void r_set_str(row** in_row, int index, char* value);

//Print Row
void print_row(row** in_row);

//Serialization
size_t pack_row(row** in_row, char** buf);
row* unpack_row(char* types, char** buf);

//Utils
size_t get_size(row** in_row);
#endif
