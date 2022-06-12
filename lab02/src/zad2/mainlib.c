#include <stdio.h>
#include <malloc.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Niepoprawna ilość argumentow\n");
        return -1;
    }
    if (strlen(argv[1]) != 1) {
        printf("Niepoprawny argument.");
        return -1;
    }

    char *specialChar = (char *) calloc(1, sizeof(char));
    char *c = (char *) calloc(1, sizeof(char));
    strcpy(specialChar, argv[1]);


    FILE *we = fopen(argv[2], "r");

    int charCounter = 0;
    int charCounterPerLine = 0;
    int charOccuredInLine = 0;
    while (fread(c, sizeof(char), 1, we) == 1) {
//        printf("%c", c[0]);
        if (c[0] == specialChar[0]) {
            charCounter++;
            charOccuredInLine = 1;
        }
        while (c[0] != '\n') {
            fread(c, sizeof(char), 1, we);
//            printf("%c", c[0]);
            if (c[0] == specialChar[0]) {
                charCounter++;
                charOccuredInLine = 1;
            }
        }
        if (charOccuredInLine)
            charCounterPerLine++;
        charOccuredInLine = 0;
    }
    printf("Znak - %c - występuje %d razy w pliku, w %d liniach.\n", specialChar[0], charCounter, charCounterPerLine);
}