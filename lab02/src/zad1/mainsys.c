#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int checkLine(char *line) {
    size_t i = 0;
    for (; i < strlen(line); i++) {
//        printf("X%cX\n", line[i]);
        if (line[i] == ' ' || line[i] == '\t' || line[i] == '\r' || line[i] == '\n')
            continue;
        else
            return 0;
    }
    return 1;
}

char *appendCharToCharArray(char *array, char a) {
    size_t len = strlen(array);
    char *ret = (char *) calloc(len + 2, sizeof(char));
    strcpy(ret, array);
    ret[len] = a;
    ret[len + 1] = '\0';
    return ret;
}


int main(int argc, char **argv) {
    char *name1 = (char *) calloc(32, sizeof(char));
    char *name2 = (char *) calloc(32, sizeof(char));
    if (argc == 1) {
        printf("Nazwa 1. pliku:");
        scanf("%s", name1);
        printf("Nazwa 2. pliku:");
        scanf("%s", name2);
    } else {
        strcpy(name1, argv[1]);
        strcpy(name2, argv[2]);
    }

//    char c;
    char *c = (char *) calloc(1, sizeof(char));

    int we = open(name1, O_RDONLY);
    int wy = open(name2, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    char *line = (char *) calloc(512, sizeof(char));

    int cnt = 1;

    while (read(we, c, 1) == 1) {
//        printf("--line %d\n", cnt);
        cnt++;
//        line = appendCharToCharArray(line, c);
        line = strcat(line, c);
        while (c[0] != '\n') {
            read(we, c, 1);
//            line = appendCharToCharArray(line, c);
            line = strcat(line, c);
        }
//        printf("%s", line);
        if (checkLine(line) == 0) {
//            printf("VALID LINE\n");
            size_t i = 0;
            for (; i < strlen(line); i++) {
                write(wy, &line[i], 1);
            }
        }
        memset(line, 0, strlen(line));
    }
}