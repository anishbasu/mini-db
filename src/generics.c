#include <string.h>
#include <stdlib.h>
#include "generics.h"
#include "util.h"
#ifndef BOOL_FLAGS
#define BOOL_FLAGS
#define false 0
#define true 1
#endif

static int def_i = 0;
static char def_c = 'a';
static char* def_s = "";

int is_type(generic** data, i_type type){
	if((*data)->type == type){
		return true;
	} else {
		return false;
	}
}

int verify_types(char* types){
	int result = true;
	for(int i = 0; i < strlen(types); i++){
		if(types[i] != 'c' && types[i] != 's' && types[i] != 'i'){
			result = false;
		}
	}
	return result;
}

int geneq(generic* gen_1, generic* gen_2){
    if(gen_1->type == gen_2->type){
        switch(gen_1->type){
            case int_t:
                return gen_1->value.int_v == gen_2->value.int_v;
            case char_t:
                return gen_1->value.char_v == gen_2->value.char_v;
            case str_t:
                return streq(gen_1->value.str_v, gen_2->value.str_v);
            default:
                return 0;
        }
    }
    return 0;
}

char format_type(generic** data){
	if(is_type(data, int_t)){
		return 'i';
	}
	else if(is_type(data, str_t)){
		return 's';
	}
	else if(is_type(data, char_t)){
		return 'c';
	} else {
		return '\0';
	}
}

i_type get_type(char type){
	if(type == 'i'){
		return int_t;
	}
	else if(type == 's'){
		return str_t;
	}
	else if(type == 'c'){
		return char_t;
	}
	return null_t;
}

void set_int(generic** g, void* value){
	int *nval = (int *) value;
	(*g)->value.int_v = *nval;
}

int get_int(generic** g) {
	return (*g)->value.int_v;
}

void set_char(generic** g, void* value){
	char *nval = (char *) value;
	(*g)->value.char_v = *nval;
}

char get_char(generic** g){
	return (*g)->value.char_v;
}

void set_str(generic** g, void* value){
	char* str_val = (char *) value;
	(*g)->value.str_v = malloc(sizeof(char) * strlen(str_val));
	strcpy((*g)->value.str_v, str_val);
}

char* get_str(generic** g){
	return (*g)->value.str_v;
}

void create_generic(generic** new_gen, i_type type, void* value){
	(*new_gen) = (generic *) malloc(sizeof(generic));
	(*new_gen)->type = type;
	switch(type){
		case int_t:
			set_int(new_gen, value);
			break;
		case char_t:
			set_char(new_gen, value);
			break;
		case str_t:
			set_str(new_gen, value);
			break;
		default:
			break;
	}
}

void create_def_generic(generic** new_gen, i_type type){
	switch(type){
		case int_t:
			create_generic(new_gen, type, &def_i);
			break;
		case char_t:
			create_generic(new_gen, type, &def_c);
			break;
		case str_t:
			create_generic(new_gen, type, def_s);
			break;
		default:
			break;
	}
}

void create_generic_str(generic** new_gen, i_type type, char* initial_cond){
    int i_val;
    switch(type){
        case int_t:
            i_val = atoi(initial_cond);
            create_generic(new_gen, type, &i_val);
            break;
        case char_t:
            create_generic(new_gen, type, initial_cond + 1);
            break;
        case str_t:
            create_generic(new_gen, type, initial_cond);
            break;
        default:
            break;
    }
}
