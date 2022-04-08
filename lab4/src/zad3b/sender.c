#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>

bool gotLastMessage = false;
int receivedSigCounter = 0;


int getSignal1(char *command) {
    if (strcmp(command, "SIGRT") == 0)
        return SIGRTMIN;
    return SIGUSR1;
}

int getSignal2(char *command) {
    if (strcmp(command, "SIGRT") == 0)
        return SIGRTMIN + 1;
    return SIGUSR2;
}

void sendSignal(int i, int pid, char *command, int signal) {
    if (strcmp(command, "SIGQUEUE") == 0) {
        const union sigval temp = {.sival_int = i};
        sigqueue(pid, signal, temp);
    } else {
        kill(pid, signal);
    }
}

void sigusr1_handler(int signo) {
    receivedSigCounter++;
}

void sigusr1SigqueueHandler(int signo, siginfo_t *info, void *context) {
    printf("Received signal id %d\n", info->si_value.sival_int);

    receivedSigCounter++;
}

void sigusr2_handler(int signo) {
    gotLastMessage = true;
}

void setupSignals(char *command) {
    sigset_t block_mask;
    sigfillset(&block_mask);
    sigdelset(&block_mask, getSignal1(command));
    sigdelset(&block_mask, getSignal2(command));

    if (sigprocmask(SIG_SETMASK, &block_mask, NULL) != 0) {
        puts("sigprocmask error");
        exit(EXIT_FAILURE);
    }


    struct sigaction action;
    if (strcmp(command, "SIGQUEUE") == 0) {
        action.sa_flags = SA_SIGINFO;
        action.sa_sigaction = sigusr1SigqueueHandler;
    } else {
        action.sa_flags = 0;
        action.sa_handler = sigusr1_handler;
    }
    sigemptyset(&action.sa_mask);

    if (sigaction(getSignal1(command), &action, NULL) == -1) {
        printf("Something went wrong -> %d\n", getSignal1(command));
        return;
    }


    action.sa_handler = sigusr2_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    if (sigaction(getSignal2(command), &action, NULL) == -1) {
        printf("Something went wrong -> %d\n", getSignal2(command));
        return;
    }
}

int main(int argc, char **argv) {
    argc--;
    argv++;

    if (argc != 3) {
        printf("Wrong number of arguments\n");
        printf("Sample usage - ./sender pid no_signals type\n");
        return -1;
    }

    int pid = atoi(argv[0]);
    int num = atoi(argv[1]);
    char *command = argv[2];
    if (pid <= 0 || num < 0) {
        printf("Invalid arguments\n");
        return 0;
    }
    setupSignals(command);

    for (int i = 0; i < num; i++) {
        sendSignal(i, pid, command, getSignal1(command));
        while (receivedSigCounter < i + 1) {
        }
    }
    sendSignal(0, pid, command, getSignal2(command));

    while (!gotLastMessage) {

    }

    printf("Sent: %d\nReceived %d\n", num, receivedSigCounter);

    return 0;
}