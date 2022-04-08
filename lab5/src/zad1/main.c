#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){

    argc--;
    argv++;

    if (argc != 1) {
        printf("Incorrect amount of arguments.");
        return -1;
    }
    char* line = calloc(128, sizeof(char));
    FILE *fp = fopen(argv[0], "r");
    if (!fp){
        fprintf(stderr, "Error opening file '%s'\n", argv[0]);
        return -1;
    }
    char sep = '=';
    char** execStrings = calloc(128, sizeof(char*));
    char* command = calloc(128, sizeof(char));
    while(fgets(line, 128,fp)){
        printf("The line: %s", line);
        char* ptr = strstr(line, &sep);
        if (!ptr){
            continue;
        }
        ptr++;
        ptr++;
        printf("found %s\n", ptr);
        free(ptr);
    }

    fclose(fp);
    return 0;
}