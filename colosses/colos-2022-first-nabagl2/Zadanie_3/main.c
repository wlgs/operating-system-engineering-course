#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sighandler(int signo, siginfo_t *siginfo, void *context) {
    printf("[CHILD]: Received SIGUSR1: value -> %d\n", siginfo->si_value.sival_int);
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    struct sigaction action;
    action.sa_sigaction = &sighandler;
    action.sa_flags = SA_SIGINFO;


    int child = fork();
    if (child == 0) {
        //zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1
        //zdefiniuj obsluge SIGUSR1 w taki sposob zeby proces potomny wydrukowal
        //na konsole przekazana przez rodzica wraz z sygnalem SIGUSR1 wartosc

        sigset_t block_mask;
        sigfillset(&block_mask);
        sigdelset(&block_mask, SIGUSR1);
        sigaction(SIGUSR1,&action, NULL);
        sleep(1); // dodałem sleepa by proces dziecko poczekał na sygnał od rodzica

    } else {
        union sigval signal;
        signal.sival_int = atoi(argv[1]);

        //wyslij do procesu potomnego sygnal przekazany jako argv[2]
        //wraz z wartoscia przekazana jako argv[1]
        printf("[PARENT]: Trying to send signal...\n");
        if (sigqueue(child, atoi(argv[2]), signal) == -1){
            fprintf(stderr, "Could not send the signal.\n");
        }
    }
    return 0;
}
