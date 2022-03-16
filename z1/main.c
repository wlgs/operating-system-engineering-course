#include <stdio.h>
#include "libz1.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>


float diffUserTime(struct rusage *start, struct rusage *end)
{
    return (end->ru_utime.tv_sec - start->ru_utime.tv_sec) +
           1e-6*(end->ru_utime.tv_usec - start->ru_utime.tv_usec) * 1000.0;
}

float diffSystemTime(struct rusage *start, struct rusage *end)
{
    return (end->ru_stime.tv_sec - start->ru_stime.tv_sec) +
           1e-6*(end->ru_stime.tv_usec - start->ru_stime.tv_usec) * 1000.0;
}

float diffRealTime(struct timeval *start, struct timeval *end){
    float elapsedTime;
    elapsedTime = (end->tv_sec - start->tv_sec)*1000.0;      // sec to ms
    elapsedTime += (end->tv_usec - start->tv_usec) / 1000.0;
    return elapsedTime;
}

int main(int argc, char **argv)
{
    struct rusage mainStart, mainEnd;
    struct timeval mainRealStart, mainRealEnd;
    gettimeofday(&mainRealStart, NULL);

    getrusage(RUSAGE_SELF, &mainStart);
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
            printf("Bad argument\n");
            return -1;
        }
    }

    getrusage(RUSAGE_SELF, &mainEnd);
    gettimeofday(&mainRealEnd, NULL);
    printf("User time: %.08f\n", diffUserTime(&mainStart, &mainEnd));
    printf("System time: %.08f\n", diffSystemTime(&mainStart, &mainEnd));
    printf("Real time: %.08f\n", diffRealTime(&mainRealStart, &mainRealEnd));

    return 0;
}