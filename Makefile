all: clean mini

mini:
	gcc generics.c rows.c tables.c util.c rownodes.c lexer.c parser.c main.c query.c database.c -o mini

clean:
	rm ./mini *.meta *.db