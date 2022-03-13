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
    int nextIndex = blockArray.index + 1;
    blockArray.index = nextIndex;
    blockArray.Block[nextIndex] = newBlock;
    return nextIndex;
};

char* issueWcCommand(int filesCount, char** fileNames){
    char* command = calloc(255, sizeof(char));
    strcpy(command, "wc ");
    for (int i = 0; i<filesCount; i++){
        strcat(command, fileNames[i]);
        strcat(command, " ");
    }
    char* filename = ".tmpfile";
    strcat(command, "> ");
    strcat(command, filename);
    

    printf("command is: %s\n", command);
    system(command);

    return filename;
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
    data[i] = '\0';

    struct Block newBlock;
    newBlock.data = (char*) calloc(strlen(data), sizeof(char));
    strcpy(newBlock.data, data);
    free(data);
    char* command = calloc(255, sizeof(char));
    strcpy(command, "rm -f ");
    strcat(command, filename);
    system(command);
    return addBlock(blockArray, newBlock);
}

void removeBlock(struct BlockArray blockArray, int index){
    free(blockArray.Block[index].data);
    free(blockArray.Block);
}