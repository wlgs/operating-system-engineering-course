#include <stdio.h>
#include "libz1.h"

int main(int argc, char **argv){
    // slice the args
    argv++;
    argc--;

    struct BlockArray blockArray = createBlockArray(10);
    char* tmpfile = issueWcCommand(argc, argv);
    saveFileToBlockArray(tmpfile, blockArray);
    

    printf("BLOCK DATA:\n");
    printf("%s\n", blockArray.Block[0].data);
    


    removeBlock(blockArray, 0);

    printf("BLOCK DATA:\n");
    printf("%s", blockArray.Block[0].data);
    return 0;
}