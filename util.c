//
//  util.c
//  MiniDB
//
//  Created by Anish Basu on 7/26/17.
//  Copyright © 2017 Anish Basu. All rights reserved.
//

#include "util.h"
#include "rownodes.h"
#include <stdlib.h>
#include <string.h>

void hexDump (char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;
    
    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);
    
    if (len == 0) {
        printf("  ZERO LENGTH\n");
        return;
    }
    if (len < 0) {
        printf("  NEGATIVE LENGTH: %i\n",len);
        return;
    }
    
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).
        
        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);
            
            // Output the offset.
            printf ("  %04x ", i);
        }
        
        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);
        
        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }
    
    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }
    
    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}

size_t get_file_size(FILE* fp){
    size_t fsize;
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return fsize;
}

size_t read_buffer(FILE* fp, char** buf, size_t size){
    *buf = (char *) malloc(size);
    size = fread(*buf, sizeof(char), size, fp);
    return size;
}

size_t write_buffer(FILE* fp, char* buf, size_t size){
    size = fwrite(buf, sizeof(char), size, fp);
    return size;
}

/*
void db_read(rownode** rowset2){
    //Read
    FILE* out_file;
    out_file = fopen("test.db", "r");
    char* buf2;
    size_t size = read_buffer(out_file, &buf2);
    
    unpack_rownodes("csi",rowset2, &buf2, size);
    printf("Rowset Unpacked\n");
}
*/
size_t pack_string(char* buf, char* string){
    size_t len = sizeof(char) * (strlen(string) + 1);
    memcpy(buf, &len, sizeof(size_t));
    memcpy(buf + sizeof(size_t), string, len);
    len+= sizeof(size_t);
    return len;
}

off_t unpack_string(char* buf, char** string){
    off_t pos = 0;
    size_t len = 0;
    memcpy(&len, buf, sizeof(size_t));
    pos+= sizeof(len);
    *string = malloc(len * sizeof(char));
    memcpy(*string, buf + pos, len);
    pos+= len;
    return pos;
}

int streq(char* str1, char* str2){
    return strcmp(str1, str2) == 0;
}

int prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

size_t mstrcpy(char** dest, char** src){
    *dest = (char *) malloc(strlen(*src));
    strcpy(*dest, *src);
    return strlen(*dest);
}
