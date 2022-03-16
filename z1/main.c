#include <stdio.h>
#include "libz1.h"

int main(int argc, char **argv)
{
    // slice the args
    argv++;
    argc--;

    int idx = 0;
    struct BlockArray mainArray;
    // main switch
    while (idx < argc)
    {
        printf("READING %s, id: %d\n", argv[idx], idx);
        if (strcmp(argv[idx], "create_table") == 0)
        {
            idx++;
            printf("Creating table of %d\n", atoi(argv[idx]));
            mainArray = createBlockArray(atoi(argv[idx]));
            idx++;
        }
        else if (strcmp(argv[idx], "remove_block") == 0)
        {
            idx++;
            printf("Removing block %d\n", atoi(argv[idx]));
            removeBlock(mainArray, atoi(argv[idx]));
            idx++;
        }
        else if (strcmp(argv[idx], "wc_files") == 0)
        {
            idx++;
            char* wcArgs = (char *) calloc(255,sizeof(char));
            strcpy(wcArgs, argv[idx]);
            while (1)
            {
                idx++;
                if (idx >= argc)
                    break;
                if (strcmp(argv[idx], "wc_files") == 0 || strcmp(argv[idx], "remove_block") == 0 || strcmp(argv[idx], "create_table") == 0)
                {
                    break;
                }
                strcat(wcArgs, " ");
                strcat(wcArgs, argv[idx]);
            }
            printf("Executing wc %s\n", wcArgs);
            char *outputFileName = wc_file(wcArgs);
            int indexAdded = saveFileToBlockArray(outputFileName, mainArray);
            printf("Saved to block at %d\n", indexAdded);
            free(wcArgs);
        }
        else
        {
            printf("Bad argument");
            return -1;
        }
    }
    printf("FINAL RES:\n");
    printf("%s\n", mainArray.Block[0].data);
    printf("%s\n", mainArray.Block[1].data);
    return 0;
}