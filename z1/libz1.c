#include "libz1.h"

// creates a new array of blocks with given size, the index is -1 so that when we add new block it goes to 0
struct BlockArray createBlockArray(int size){
    struct BlockArray array;
    array.index = -1;
    array.Block = (struct Block*) calloc(size, sizeof(struct Block));
    return array;
};

// adds new block to the array of blocks, nextly increments index, then returns the index of inserted block 
int addBlock(struct BlockArray blockArray, struct Block newBlock){
    blockArray.index++;
    blockArray.Block[blockArray.index] = newBlock;
    return blockArray.index;
};

char* wc_file(char* fileName){
    char* command = calloc(255, sizeof(char));
    strcpy(command, "wc ");
    strcat(command, fileName);
    char* output = ".tmpfile";
    strcat(command, " > ");
    strcat(command, output);
    system(command);
    return output;
}

int saveFileToBlockArray(char* filename, struct BlockArray blockArray){

    FILE *file = fopen(filename, "r");
    if(file==NULL){
        printf("Error opening the file\n");
        exit(-1);
    }

    // get the file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    //allocate the memory for temp res
    char *data = (char*) calloc((size_t) size, sizeof(char));

    int i=0;
    char ch;
    //we read char by char
    while(!feof(file)) {
        if((ch = (char) fgetc(file)) != -1){
            data[i++] = ch;
        }
    }
    fclose(file);
    free(file);
    data[i] = '\0';

    struct Block newBlock;
    newBlock.data = (char*) calloc(strlen(data), sizeof(char));
    strcpy(newBlock.data, data);
    free(data);
    char* command = calloc(255, sizeof(char));
    strcpy(command, "rm -f ");
    strcat(command, filename);
    system(command);
    free(command);

    int index = addBlock(blockArray, newBlock);
    return index;
}

void removeBlock(struct BlockArray blockArray, int index){
    printf("OLD:%s\n", blockArray.Block[index].data);
    blockArray.Block[index].data[0]='\0'; //reset the char
    free(blockArray.Block[index].data);
    printf("NEW:%s\n", blockArray.Block[index].data);
}