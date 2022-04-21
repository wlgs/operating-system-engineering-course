#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define PIPE "./squareFIFO"

int main() {
    int val = 0;
    int fd = open(PIPE, O_RDONLY);
    char buff[128];
    read(fd, &buff, sizeof(buff) );
    val = atoi(buff);
    /***********************************
    * odczytaj z potoku nazwanego PIPE zapisana tam wartosc i przypisz ja do zmiennej val
    * posprzataj
    ************************************/
    close(fd);
    printf("%d square is: %d\n", val, val*val);
    return 0;
}
