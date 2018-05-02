#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include "parser.h"
#include "util.h"
#include <sys/stat.h>
/*
 For char send &value
 For int send &value
 For string send value
 */
int file_exists (char *filename)
{
    struct stat   buffer;
    return (stat (filename, &buffer) == 0);
}

int main(int argc, char* argv[]) {
    if(argc != 2){
        printf("Please include database name: ./mini <db_name>\n");
        return 1;
    }
    database* db = create_database(argv[1]);
    char file_name[255];
    snprintf(file_name, 255, "%s.meta",db->name);
    if(file_exists(file_name)){
        read_files(&db);
    }
    
    char* query;
    size_t len;
    ssize_t read_in;
    while(1){
        printf("%s=>", db->name);
        if((read_in = getline(&query, &len, stdin)) != -1){
            if(read_in <= 2){
                write_files(&db);
                return 0;
            }
            interactive(&db, query);
        }
    }
}
