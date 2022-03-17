#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include "libz1.h"

clock_t clock_t_begin, clock_t_end;
struct tms times_start_buffer, times_end_buffer;
FILE *timerFile;

void start_timer()
{
    clock_t_begin = times(&times_start_buffer);
}

void stop_timer()
{
    clock_t_end = times(&times_end_buffer);
}

double calc_time(clock_t s, clock_t e)
{
    return ((long int)(e - s) / (double)sysconf(_SC_CLK_TCK));
}

void print_times(const char *operation)
{
    fprintf(timerFile, "%20s real %.3fs, user %.3fs, sys %.3fs\n",
            operation,
            calc_time(clock_t_begin, clock_t_end),
            calc_time(times_start_buffer.tms_cutime, times_end_buffer.tms_cutime),
            calc_time(times_start_buffer.tms_cstime, times_end_buffer.tms_cstime));
}

int main(int argc, char **argv)
{
    // slice the args
    argv++;
    argc--;
    timerFile = fopen("raport_generated.txt", "a");
    if (timerFile == NULL)
    {
        printf("Could not open report.txt in write mode");
        return -1;
    }
    int idx = 0;
    struct BlockArray mainArray;
    // main switch
    while (idx < argc)
    {
        // printf("READING %s, id: %d\n", argv[idx], idx);
        if (strcmp(argv[idx], "create_table") == 0)
        {
            idx++;
            printf("Creating table of %d\n", atoi(argv[idx]));

            start_timer();
            mainArray = createBlockArray(atoi(argv[idx]));
            stop_timer();
            print_times("create_table");

            idx++;
        }
        else if (strcmp(argv[idx], "remove_block") == 0)
        {
            idx++;
            printf("Removing block %d\n", atoi(argv[idx]));
            start_timer();
            removeBlock(mainArray, atoi(argv[idx]));
            stop_timer();
            print_times("remove_block");
            idx++;
        }
        else if (strcmp(argv[idx], "wc_files") == 0)
        {
            idx++;
            char *wcArgs = (char *)calloc(255, sizeof(char));
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

            start_timer();
            char *outputFileName = wc_file(wcArgs);
            stop_timer();
            print_times("wc_files");

            start_timer();
            int indexAdded = saveFileToBlockArray(outputFileName, mainArray);
            stop_timer();
            print_times("add_to_block");
            printf("Saved to block at %d\n", indexAdded);
            free(wcArgs);
        }
        else
        {
            printf("Bad argument\n");
            return -1;
        }
    }
    fclose(timerFile);
    return 0;
}