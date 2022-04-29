#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define PIPE_READ_IDX 0
#define PIPE_WRITE_IDX 1

#define MAX_LINES 10
#define MAX_ARGUMENTS 20


int get_line_cnt(char *line, int i) {
    if (i == 0) return line[8] - '0';
    else return line[9] - '0';
}

char **read_cmd(char *line) {
    char **commands = (char **) calloc(MAX_ARGUMENTS, sizeof(char *));
    char *seq = strtok(line, "=");
    int ctr = 0;
    while ((seq = strtok(NULL, "|")) != NULL) {
        commands[ctr++] = seq;
    }
    return commands;
}

char **read_args(char *command, char *path) {
    char **args = (char **) calloc(256, sizeof(char *));
    char *arg = strtok(command, " ");
    strcpy(path, arg);
    int ctr = 0;
    args[ctr++] = arg;
    while ((arg = strtok(NULL, " ")) != NULL) {
        args[ctr++] = arg;
    }
    args[ctr] = NULL;
    return args;
}

int *read_lines_main(char *line) {
    char **lines = (char **) calloc(MAX_ARGUMENTS, sizeof(char *));
    char *arg = strtok(line, "|");
    int ctr = 0;
    lines[ctr++] = arg;
    while ((arg = strtok(NULL, "|")) != NULL) {
        lines[ctr++] = arg;
    }
    static int lines_num[MAX_ARGUMENTS];
    int i = 0;
    while (lines[i] != NULL) {
        lines_num[i] = get_line_cnt(lines[i], i);
        i++;
    }
    lines_num[i] = -1;
    return lines_num;
}


void read_main(FILE *file) {
    char **lines = (char **) calloc(MAX_LINES, sizeof(char *));   // commands: lines with "="
    char **commands;
    char **args;
    char *line = (char *) calloc(256, sizeof(char));
    char *current_line;
    int *lines_num;
    int line_ctr = 0;
    int lines_number;
    int i;
    while (fgets(line, 256 * sizeof(char), file)) {
        printf("line\n: %s", line);
        if (strstr(line, "=")) {
            char *line_copy = (char *) calloc(256, sizeof(char));
            strcpy(line_copy, line);
            lines[line_ctr++] = line_copy;
        } else {
            lines_num = read_lines_main(line);
            printf("lines to execute: \n");
            i = 0;
            while (lines_num[i] != -1) {
                printf("lin %d\n", lines_num[i]);
                i++;
            }
            int pipe_in[2];
            int pipe_out[2];
            if (pipe(pipe_out) != 0) {
                fprintf(stderr,"Error while creating a pipe!\n");
                exit(1);
            }
            lines_number = 0;
            while (lines_num[lines_number] != -1) lines_number++;
            for (i = 0; i < lines_number; i++) {
                current_line = lines[lines_num[i]];
                printf("reading line %d:  %s \n", i, current_line);
                commands = read_cmd(lines[lines_num[i]]);
                int m = 0;
                while (commands[m] != NULL) {
                    printf("command to execute [%d]: %s\n", m + 1, commands[m]);
                    m++;
                }
                for (int j = 0; j < m; j++) {
                    pid_t pid = fork();
                    if (pid == 0) {
                        if (j == 0 && i == 0) {
                            close(pipe_out[PIPE_READ_IDX]);
                            dup2(pipe_out[PIPE_WRITE_IDX], STDOUT_FILENO);
                        }

                        else if (j == m - 1 && i == lines_number - 1) {
                            close(pipe_out[PIPE_READ_IDX]);
                            close(pipe_out[PIPE_WRITE_IDX]);
                            close(pipe_in[PIPE_WRITE_IDX]);
                            dup2(pipe_in[PIPE_READ_IDX], STDIN_FILENO);
                        } else {
                            close(pipe_in[PIPE_WRITE_IDX]);
                            close(pipe_out[PIPE_READ_IDX]);
                            dup2(pipe_in[PIPE_READ_IDX], STDIN_FILENO);
                            dup2(pipe_out[PIPE_WRITE_IDX], STDOUT_FILENO);
                        }
                        char path[256];
                        args = read_args(commands[j], path);
                        printf("execvp:\npath: %s\n", path);
                        // print args
                        m = 0;
                        while (args[m] != NULL) {
                            printf("arg%d: %s\n", m + 1, args[m]);
                            m++;
                        }
                        if (execvp(path, args) == -1) {
                            fprintf(stderr,"ERROR in exec\n");
                            exit(1);
                        }
                    } else {
                        close(pipe_in[PIPE_WRITE_IDX]);
                        pipe_in[PIPE_READ_IDX] = pipe_out[PIPE_READ_IDX];
                        pipe_in[PIPE_WRITE_IDX] = pipe_out[PIPE_WRITE_IDX];

                        if (pipe(pipe_out) != 0) {
                            fprintf(stderr,"Error while moving pipe!\n");
                            exit(1);
                        }
                    }
                }
            }
        }
        int status = 0;
        pid_t wpid;
        while ((wpid = wait(&status)) != -1);
        printf("-== Executing ended. ==-\n");
    }

}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr,"Wrong number of arguments!\n");
        exit(1);
    }
    char *path = argv[1];
    FILE *commands = fopen(path, "r");
    if (commands == NULL) {
        fprintf(stderr,"Cannot open file\n");
        exit(1);
    }
    read_main(commands);
    fclose(commands);

}