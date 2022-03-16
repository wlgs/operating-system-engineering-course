#include "libz1.h"

struct BlockArray createBlockArray(int size)
{
    struct BlockArray array;
    array.maxIndex = size - 1;
    array.Block = (struct Block *)calloc(size, sizeof(struct Block));
    for (int i = 0; i < size; i++)
    {
        array.Block[i].empty = 1;
    }
    return array;
};

int getNextFreeIndex(struct BlockArray blockArray)
{
    for (int i = 0; i <= blockArray.maxIndex; i++)
    {
        if (blockArray.Block[i].empty)
            return i;
    }
    return -1;
}

// adds new block to the array of blocks, nextly increments index, then returns the index of inserted block
int addBlock(struct BlockArray blockArray, struct Block newBlock)
{
    int nextIndex = getNextFreeIndex(blockArray);
    blockArray.Block[nextIndex] = newBlock;
    return nextIndex;
};

char *wc_file(char *wcArgs)
{
    char *command = calloc(255, sizeof(char));
    strcpy(command, "wc ");
    strcat(command, wcArgs);
    char *output = ".tmpfile";
    strcat(command, " > ");
    strcat(command, output);
    system(command);
    free(command);
    return output;
}

int saveFileToBlockArray(char *filename, struct BlockArray blockArray)
{

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening the file\n");
        exit(-1);
    }

    // get the file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocate the memory for temp res
    char *data = (char *)calloc((size_t)size, sizeof(char));

    int i = 0;
    char ch;
    // we read char by char
    while (!feof(file))
    {
        if ((ch = (char)fgetc(file)) != -1)
        {
            data[i++] = ch;
        }
    }
    fclose(file);
    i--;
    i--;
    data[i] = '\0';
    // printf("%s\n",data);
    struct Block newBlock;
    newBlock.data = (char *)calloc(strlen(data), sizeof(char));
    newBlock.empty = 0;
    strcpy(newBlock.data, data);
    free(data);
    char* command = (char *) calloc(128, sizeof(char));
    strcpy(command, "rm -f ");
    strcat(command, filename);
    system(command);
    free(command);

    int index = addBlock(blockArray, newBlock);
    return index;
}

void removeBlock(struct BlockArray blockArray, int index)
{
    blockArray.Block[index].data[0] = '\0'; // reset the char
    free(blockArray.Block[index].data);
    blockArray.Block[index].empty = 1;
}