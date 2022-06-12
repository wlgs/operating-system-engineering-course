#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

const int SIGNAL_ID = SIGUSR1;

void signalHandle(int sigtype) {
    printf("Signal received: %d\n", sigtype);
}

void maskSignal() {
    sigset_t blocked_set;
    sigemptyset(&blocked_set);
    sigaddset(&blocked_set, SIGNAL_ID);
    sigprocmask(SIG_BLOCK, &blocked_set, NULL);
}

void checkSignalPending() {
    sigset_t pending_set;
    sigpending(&pending_set);
    if (sigismember(&pending_set, SIGNAL_ID))
        printf("Signal with id %d is pending.\n", SIGNAL_ID);
    else
        printf("Signal with id %d is NOT pending.\n", SIGNAL_ID);
}

int main(int argc, char **argv) {
    argv++;
    argc--;
    if (argc != 1) {
        printf("Wrong amount of arguments.");
        return -1;
    }
    char *command = argv[0];
    if (strcmp(command, "ignore") == 0) {
        signal(SIGNAL_ID, SIG_IGN);
    } else if (strcmp(command, "handler") == 0) {
        signal(SIGNAL_ID, signalHandle);
    } else if (strcmp(command, "mask") == 0 || strcmp(command, "pending") == 0) {
        maskSignal();
    }
    printf("Trying to raise the signal...\n");
    raise(SIGNAL_ID);

    printf("Main process: ");
    checkSignalPending();

    int pid = fork();
    if (pid == 0) //child
    {
        if (strcmp(command, "pending") == 0) {
            printf("[CHILD] Trying to raise the signal...\n");
            raise(SIGNAL_ID);
        }
        printf("Child process: ");
        checkSignalPending();
        if (execl("./main2", "main2", NULL) == -1) {
            fprintf(stderr, "Error executing main2");
            exit(1);
        }
    }

    wait(NULL);
    return 0;
}