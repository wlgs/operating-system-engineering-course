#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

void sigusr1_handler(int signo, siginfo_t *info, void *context) {
    if (signo != SIGUSR1)
        return;
    if (info->si_code == SI_USER)
        printf("SIGUSR1 by kill from userid=%d\n", info->si_uid);
}

void runTest1(void) {
    struct sigaction action;
    action.sa_sigaction = sigusr1_handler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);

    if (sigaction(SIGUSR1, &action, NULL) == -1) {
        printf("Something went wrong catching signal.\n");
        return;
    }

    kill(getpid(), SIGUSR1);
}

void sigchildHandler(int signo, siginfo_t *info, void *context) {
    if (signo != SIGCHLD) {
        return;
    }
    printf("SIGCHLD - si_status=%d\n", info->si_status);
}

int runTest2(void) {
    struct sigaction action;
    action.sa_sigaction = sigchildHandler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);

    if (sigaction(SIGCHLD, &action, NULL) == -1) {
        printf("Something went wrong catching signal.\n");
        return 0;
    }
    pid_t childPid = fork();
    if (childPid == 0)
        exit(-1);
    wait(NULL);
    return 0;
}


void sigfpeHandler(int signo, siginfo_t *info, void *context) {
    if (signo != SIGFPE)
        return;

    if (info->si_code == FPE_INTDIV) {
        printf("SIGFPE - si_ddr=%p\n", info->si_addr);
        exit(EXIT_FAILURE);
    }
}

void runTest3(void) {
    struct sigaction action;
    action.sa_sigaction = sigfpeHandler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);

    if (sigaction(SIGFPE, &action, NULL) == -1) {
        printf("Cant catch SIGFPE\n");
        return;
    }

    int c = 0;
    printf("%d", 5 / c);
}

void runTests(void) {
    runTest1();
    runTest2();
    runTest3();
}

int main(int argc, char **argv) {
    argc--;
    argv++;
    runTests();
    return 0;
}