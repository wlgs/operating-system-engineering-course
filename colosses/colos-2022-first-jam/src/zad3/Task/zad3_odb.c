#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int lpar, char *tab[]) {
    int w1;
    char buf[1024];

    if (mkfifo("potok1", 0666) == -1) {
        fprintf(stderr, "could not create pipe -> %d\n ", errno);
        return -1;
    }
    printf("Created pipe.\n");

   w1 = open("potok1", O_RDONLY);
    // 1) utworzyc potok nazwany 'potok1'
    // 2) wyswietlac liczby otrzymywane z potoku

    while (1) {
        read(w1, &buf, sizeof(buf));
        sleep(1);
        printf("otrzymano: %s\n", buf);
        fflush(stdout);
    }
    close(w1);
    return 0;
}
