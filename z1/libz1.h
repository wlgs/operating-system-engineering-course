#ifndef libz1_h
#define libz1_h
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct ArrayOfBlocks{
    int index;
    struct Block *arrayOfBlocks;
};

struct Block{
    char *wcl; // result of count lines
    char *wcw; // result of count words
    char *wcc; // result of count characters
};

struct ArrayOfBlocks createArrayOfBlocks(int size);
int addBlock(struct ArrayOfBlocks arrayOfBlocks, struct Block newBlock);



