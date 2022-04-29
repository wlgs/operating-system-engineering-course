#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: main fifo filename characters_amount\n");
        return 0;
    }
    srand(time(NULL));
    size_t chars_per_read = atoi(argv[3]);
    FILE *f_pipe = fopen(argv[1], "w");
    if (!f_pipe) {
        fprintf(stderr, "error opening fifo");
        exit(1);
    }
    FILE *f_read = fopen(argv[2], "r");
    if (!f_read) {
        fprintf(stderr, "error opening file in");
        exit(1);
    }
    char buffer[256];
    char *mini_data = calloc(chars_per_read + 1, sizeof(char));
    int pid = getpid();
    size_t len;
    while ((len = fread(mini_data, sizeof(char), chars_per_read, f_read)) > 0) {
        mini_data[len] = '\0';
        sprintf(buffer, "#%d#%s", pid, mini_data);
        fwrite(buffer, sizeof(char), strlen(buffer), f_pipe);
        sleep((rand() % 2) + 1);
    }
    fflush(f_pipe);
    fclose(f_pipe);
    fclose(f_read);
    free(mini_data);
    return 0;
}