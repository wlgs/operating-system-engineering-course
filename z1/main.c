#include <stdio.h>
#include "libz1.h"
#include <stdio.h>
#include <sys/times.h>

double timeDifference(clock_t t1, clock_t t2){
    return ((double)(t2 - t1) / sysconf(_SC_CLK_TCK));
}

void writeResult(FILE* resFile, clock_t start, clock_t end, struct tms* t_start, struct tms* t_end){
    printf("\tREAL_TIME: %fl\n", timeDifference(start,end));
    printf("\tUSER_TIME: %fl\n", timeDifference(t_start->tms_utime, t_end->tms_utime));
    printf("\tSYSTEM_TIME: %fl\n", timeDifference(t_start->tms_stime, t_end->tms_stime));

    fprintf(resFile, "\tREAL_TIME: %fl\n", timeDifference(start, end));
    fprintf(resFile, "\tUSER_TIME: %fl\n", timeDifference(t_start->tms_utime, t_end->tms_utime));
    fprintf(resFile, "\tSYSTEM_TIME: %fl\n", timeDifference(t_start->tms_stime, t_end->tms_stime));
}

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
    return 0;
}