#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("Podaj poprawna ilosc argumentow\n");
        return -1;
    }
    printf("PID głównego programu: %d\n", (int) getpid());
    argv++;
    int n = atoi(argv[0]);
    if (n < 1) {
        printf("Podaj poprawna ilosc procesow\n");
        return -1;
    }
    pid_t child_pid;
    for (int i = 1; i <= n; i++) {
        child_pid = fork();
        if (child_pid == 0) {
            printf("Wypisuję napis z procesu potomnego PID: %d\n", (int) getpid());
            return 1;
        }

    }
    return 1;
}