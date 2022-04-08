#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

int receivedSigCounter = 0;
int sender_pid = 0;

void sigusr1Handler(int signo) {
    receivedSigCounter++;
}

void sigusr2Handler(int signo, siginfo_t *info, void *context) {
    sender_pid = info->si_pid;
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
    action.sa_handler = sigusr1Handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    if (sigaction(getSignal1(command), &action, NULL) == -1) {
        printf("Cant catch signal: %d\n", getSignal1(command));
        return;
    }


    action.sa_sigaction = sigusr2Handler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);

    if (sigaction(getSignal2(command), &action, NULL) == -1) {
        printf("Cant catch signal: %d\n", getSignal2(command));
        return;
    }
}

int main(int argc, char **argv) {
    argc--;
    argv++;

    if (argc != 1) {
        printf("Wrong number of arguments\n");
        printf("Sample usage - ./catcher type\n");
        return -1;
    }

    printf("PID: %d\n", getpid());
    char *command = argv[0];
    setupSignals(command);
    while (!sender_pid);
    printf("Signals received = %d\n", receivedSigCounter);

    for (int i = 0; i < receivedSigCounter; i++)
        sendSignal(i, sender_pid, command, getSignal1(command));
    sendSignal(0, sender_pid, command, getSignal2(command));

    return 0;
}