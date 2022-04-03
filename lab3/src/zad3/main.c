#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char *updatePath(char *path, char *currentPath) {
    char *new = calloc(sizeof(char), strlen(path) + strlen(currentPath));
    sprintf(new, "%s/%s", path, currentPath);
    return new;
}

int checker(FILE *file, char *str) {
    char *line = calloc(sizeof(char), 256);
    while (fgets(line, 256 * sizeof(char), file)) {
        if (strstr(line, str)) {
            return 1;
        }
    }
    return 0;
}

void searchDir(char *dirName, char *str, int depth) {
    DIR *dir;
    FILE *file;
    struct dirent *dp;
    char *path;
    char *extension;
    if (depth == 0)
        exit(0);
    if ((dir = opendir(dirName)) == NULL) {
        perror("Cannot open directory");
        exit(1);
    }
    while ((dp = readdir(dir))) {
        if ((strcmp(dp->d_name, "..") == 0) || (strcmp(dp->d_name, ".") == 0)) continue;
        path = updatePath(dirName, dp->d_name);
        extension = strrchr(dp->d_name, '.');
        if (dp->d_type == DT_DIR && fork() == 0) {
            searchDir(path, str, depth - 1);
            exit(0);
        } else if (extension != NULL && strcmp(extension, ".txt") == 0) {
            file = fopen(path, "r");
            if (file == NULL) {
                perror("Cannot open file");
                exit(-1);
            }
            if (checker(file, str) == 1) {
                printf("\npath: %s\n", path);
                printf("PID:  %d \nPPID: %d\n", getpid(), getppid());
            }
        }
        wait(NULL);
        free(path);
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    argc--;
    argv++;
    if (argc != 3) {
        printf("Wrong number of arguments.");
        return -1;
    }
    char *dirName = argv[0];
    char *text = argv[1];
    int depth = atoi(argv[2]);
    searchDir(dirName, text, depth);
    free(dirName);
    free(text);
    return 0;
}