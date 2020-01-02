#ifndef GENERICS
#define GENERICS

typedef enum _i_type {
		int_t,
		str_t,
		char_t,
		null_t
	} i_type;

typedef struct _generic {
	i_type type;

	union {
		int int_v;
		char* str_v;
		char char_v;
	} value;
} generic;

typedef struct _generic generic;
//Type Checking
extern int is_type(generic** data, i_type type);
extern int verify_types(char* types);
extern char format_type(generic** data);
extern i_type get_type(char type);

//Comparator
int geneq(generic* gen_1, generic* gen_2);

//Integer Get/Set
extern void set_int(generic** g, void* value);
extern int get_int(generic** g);

//Character Get/Set
extern void set_char(generic** g, void* value);
extern char get_char(generic** g);

//String Get/Set
extern void set_str(generic** g, void* value);
extern char* get_str(generic** g);

//Constructor
extern void create_generic(generic** new_gen, i_type type, void* value);
extern void create_def_generic(generic** new_gen, i_type type);
extern void create_generic_str(generic** new_gen, i_type type, char* initial_cond);
#endif
