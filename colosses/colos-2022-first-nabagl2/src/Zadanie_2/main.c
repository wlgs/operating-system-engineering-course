#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>


int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Not a suitable number of program parameters\n");
        return (1);
    }
    char** argvchild = argv;
    argvchild[0] = "./calc";

    pid_t child = fork();
    if (child == 0){
        if(execvp("./calc", argvchild) == -1){
            fprintf(stderr, "Could not execute ./calc\n");
        }

    }
    //utworz proces potomny w ktorym wykonasz program ./calc z parametrami argv[1], argv[2] oraz argv[3]
    //odpowiednio jako pierwszy operand, operacja (+-x/) i drugi operand 
    //uzyj tablicowego sposobu przekazywania parametrow do programu 
    return 0;
}
