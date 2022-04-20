#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>


void sighandler(int signo, siginfo_t* siginfo, void* context)
{
    int value = siginfo->si_value.sival_int;
    if (signo == SIGUSR1)
        printf("Received signal: SIGUSR1. Value: %d\n", value);
    else
        printf("Received signal: SIGUSR2. Value: %d\n", value);
}

int main(int argc, char* argv[]) {

    if(argc != 3){
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    sigset_t block_mask;
    sigfillset(&block_mask); // add all signals to block_mask
    // sigemptyset(&block_mask);
    // sigaddset(&block_mask, SIGINT);

    sigdelset(&block_mask, SIGUSR1); // delete sigusr1 from block_mask
    sigdelset(&block_mask, SIGUSR2); // delete sigusr2 from block_mask

    sigprocmask(SIG_SETMASK, &block_mask,NULL); // set block_mask

    struct sigaction action;
    action.sa_sigaction = &sighandler;
    action.sa_flags = SA_SIGINFO;

    if (sigaction(SIGUSR1,&action, NULL)==-1){
        fprintf(stderr, "Failed to setup SIGUSR1 Handler.");
    }
    if (sigaction(SIGUSR2,&action, NULL)==-1){
        fprintf(stderr, "Failed to setup SIGUSR1 Handler.");
    }

    int child = fork();
    if(child == 0) {
        sleep(1);
    }
    else {
        //wyslij do procesu potomnego sygnal przekazany jako argv[2]
        //wraz z wartoscia przekazana jako argv[1]

        union sigval value;
        value.sival_int = atoi(argv[1]);

        if (sigqueue(child, atoi(argv[2]), value) == -1){
            fprintf(stderr, "Failed to send the signal.\n");
        }
    }
    wait(NULL);
    return 0;
}
