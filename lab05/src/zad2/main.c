#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_by_mode(char *mode) {
    char *command;
    FILE *file;
    if (strcmp(mode, "date") == 0)
        command = "echo | mail -f | tail +2 | head -n -1 | tac";
    else if (strcmp(mode, "sender") == 0)
        command = "echo | mail -f | tail +2 | head -n -1 | sort -k 2";
    else {
        printf("Wrong mode! Choose: [date] or [sender].\n");
        exit(1);
    }
    file = popen(command, "r");
    if (file == NULL) {
        fprintf(stderr, "popen error");
        exit(1);
    }
    printf("\nSORTED EMAILS: ");
    char line[256];
    while (fgets(line, 256, file) != NULL) {
        printf("%s\n", line);
    }
}

void send_mail(char *address, char *object, char *content) {
    FILE *file;
    char command[256];
    printf("Sending mail...\n");
    snprintf(command, sizeof(command), "echo %s | mail -s %s %s", content, object, address);
    printf("%s\n", command);
    file = popen(command, "r");
    if (file == NULL) {
        fprintf(stderr, "popen error");
        exit(1);
    }
    printf("-= Mail was sent. =-");
}

int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 4) {
        fprintf(stderr, "Wrong number of arguments!");
        exit(1);
    }
    if (argc == 2) {
        fprintf(stderr, "Print emails ordered by [%s]...\n", argv[1]);
        print_by_mode(argv[1]);
    } else {
        char *address = argv[1];
        char *subject = argv[2];
        char *content = argv[3];
        send_mail(address, subject, content);
    }

}