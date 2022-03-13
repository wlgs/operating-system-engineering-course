#ifndef libz1_h
#define libz1_h
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct BlockArray{
    int index;
    struct Block *Block;
};

struct Block{
    char *data; // result of wc command
};

struct BlockArray createBlockArray(int size);
int addBlock(struct BlockArray blockArray, struct Block newBlock);
char* wc_file(char* fileName);
int saveFileToBlockArray(char* filename, struct BlockArray blockArray);
void removeBlock(struct BlockArray blockArray, int index);