#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/times.h>

clock_t clock_t_begin, clock_t_end;
struct tms times_start_buffer, times_end_buffer;
FILE *timerFile;

void start_timer() {
    clock_t_begin = times(&times_start_buffer);
}

void stop_timer() {
    clock_t_end = times(&times_end_buffer);
}

double calc_time(clock_t s, clock_t e) {
    return ((long int) (e - s) / (double) sysconf(_SC_CLK_TCK));
}

void print_times(long double rectangleWidth, int n) {
    fprintf(timerFile, "Rectanglewidth: %.12Lf, Processes: %d - real %.3fs, user %.3fs, sys %.3fs\n",
            rectangleWidth,
            n,
            calc_time(clock_t_begin, clock_t_end),
            calc_time(times_start_buffer.tms_cutime, times_end_buffer.tms_cutime),
            calc_time(times_start_buffer.tms_cstime, times_end_buffer.tms_cstime));
}


long double f(long double x) {
    return 4 / (x * x + 1);
}

void integrate(long double start, long double end, long double stepSize, int idx) {
    char *filename = (char *) calloc(16, sizeof(char));
    strcpy(filename, "./w/");
    strcat(filename, "w");
    char *idc = (char *) calloc(16, sizeof(char));
    sprintf(idc, "%d", idx);
    strcat(filename, idc);
    long double result = 0;
    start += stepSize / 2.0;
    while (start <= end) {
        result += f(start) * stepSize;
        start += stepSize;
    }
    FILE *f = fopen(filename, "w");
    fprintf(f, "%.16Lf", result);
    fclose(f);
    free(filename);
    free(idc);
}


int main(int argc, char **argv) {
    timerFile = fopen("raport.txt", "a");
    if (timerFile == NULL) {
        printf("Could not open report.txt in write mode");
        return -1;
    }
    start_timer();

    argc--;
    argv++;
    if (argc != 2) {
        printf("Wrong number of arguments.");
        return -1;
    }
    char *errPtr = NULL;
    long double arg1 = strtod(argv[0], &errPtr);
    int arg2 = atoi(argv[1]);
    long double start = 0;
    long double end = 1;
    long double stepSize = (end - start) / (arg2) * 1.0;

    printf("Width of rectangle: %.16Lf\nNumber of processes: %d\n", arg1, arg2);

    for (int i = 1; i <= arg2; i++) {
        pid_t childPid = fork();
        if (childPid == 0) {
            // we are in child (xddd)
            integrate(start, start + stepSize, arg1, i);
            return 0;
        }
        start += stepSize;
    }

    wait(NULL);

    char *filename = (char *) calloc(16, sizeof(char));
    char *idc = (char *) calloc(16, sizeof(char));
    long double fres = 0;

    for (int i = 1; i <= arg2; i++) {
        strcpy(filename, "./w/");
        strcat(filename, "w");
        sprintf(idc, "%d", i);
        strcat(filename, idc);
        FILE *f = fopen(filename, "r");
        long double fval;
        fscanf(f, "%Lf", &fval);
        fres += fval;
        fclose(f);
    }
    free(filename);
    free(idc);
    stop_timer();
    printf("\nCalculated value: %Lf\n", fres);
    print_times(arg1, arg2);
    printf("Times saved in raport.txt\n");
    return 0;
}