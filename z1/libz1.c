#include "libz1.h"

// creates a new array of blocks with given size, the index is -1 so that when we add new block it goes to 0
struct ArrayOfBlocks createArrayOfBlocks(int size){
    struct ArrayOfBlocks array;
    array.arrayOfBlocks = (struct Block*) calloc(size, sizeof(struct Block));
    array.index = -1;
    return array;
};

// adds new block to the array of blocks, nextly increments index, then returns the index of inserted block 
int addBlock(struct ArrayOfBlocks arrayOfBlocks, struct Block newBlock){
    int nextIndex = arrayOfBlocks.index + 1;
    arrayOfBlocks.arrayOfBlocks[nextIndex] = newBlock;
    return nextIndex;
};

