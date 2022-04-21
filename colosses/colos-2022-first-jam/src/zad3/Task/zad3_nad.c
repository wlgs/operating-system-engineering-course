#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>


int i = 0;
int pd;

void sighandler(int signo, siginfo_t *siginfo, void *context) {
    char buf[32];
    sprintf(buf, "%d", i);

    if (signo == SIGINT) {
        write(pd, &buf, sizeof(buf));
        printf("\nSending: [%d]\n", i);
    }
}

/*
program przy kazdym uzyciu klawiszy ctrl-c ma wyslac przez potok
nazwany 'potok1' zawarto�� zmiennej 'i' */

//
//

int main(int lpar, char *tab[]) {

    struct sigaction action;
    action.sa_sigaction = &sighandler;
    action.sa_flags = SA_SIGINFO;

    pd = open("potok1", O_WRONLY);

    if (sigaction(SIGINT, &action, NULL) == -1) {
        fprintf(stderr, "CANT SETUP SIGNAL HANDLER.");
        return -1;
    }

    while (1) {
        printf("%d\n", i++);
        fflush(stdout);
        sleep(1);
    }

    close(pd);
    return 0;
}
