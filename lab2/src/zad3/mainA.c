#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// GLOBAL BECAUSE OF RECURSION
// THIS PROGRAM IS BASED ON opendir(), readdir() and stat family

int cntFiles = 0;
int cntDir = 0;
int cntFifo = 0;
int cntSock = 0;
int cntSlink = 0;
int cntCharD = 0;
int cntBlockD = 0;

void printFileInfo(const char *filename, const struct stat *stats) {
    char *buffer = (char *) calloc(512, sizeof(char));
    char *res = realpath(filename, buffer);
    printf("\tPath: \t%s\n", res);
    printf("\tType: \t");
    if (S_ISREG(stats->st_mode)) {
        cntFiles++;
        printf("file\n");
    } else if (S_ISDIR(stats->st_mode)) {
        cntDir++;
        printf("dir\n");
    } else if (S_ISLNK(stats->st_mode)) {
        cntSlink++;
        printf("slink\n");
    } else if (S_ISCHR(stats->st_mode)) {
        cntCharD++;
        printf("char dev\n");
    } else if (S_ISBLK(stats->st_mode)) {
        cntBlockD++;
        printf("block dev\n");
    } else if (S_ISFIFO(stats->st_mode)) {
        cntFifo++;
        printf("fifo\n");
    } else if (S_ISSOCK(stats->st_mode)) {
        cntSock++;
        printf("sock\n");
    }
    printf("\tFile size: \t%ld\n", stats->st_size);
    printf("\tLast access: \t%s", ctime(&stats->st_atime));
    printf("\tLast modification: \t%s", ctime(&stats->st_mtime));
    printf("\tSymbolic links: \t%ld\n", stats->st_nlink);
}

void search(char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("Unable to open directory at %s", path);
        exit(-1);
    }
    struct dirent *file;
    char *filePath = (char *) calloc(512, sizeof(char));
    while ((file = readdir(dir)) != NULL) {
        struct stat stats;
        strcpy(filePath, path);
        strcat(filePath, "/");
        strcat(filePath, file->d_name);
        if (stat(filePath, &stats) < 0) {
            printf("Unable to find stats about file: %s", filePath);
            exit(-1);
        }
        if (strcmp(file->d_name, ".") == 0)
            continue;
        else if (strcmp(file->d_name, "..") == 0)
            continue;
        if (S_ISDIR(stats.st_mode))
            search(filePath);
        printf("\tFile name: \t%s\n", file->d_name);
        printFileInfo(filePath, &stats);
    }
    closedir(dir);
}

int main(int argc, char **argv) {
    argv++;
    search(argv[0]);
    printf("- - - Summary - - -\n");
    printf("\t%d \tfile(s).\n", cntFiles);
    printf("\t%d \tdirectorie(s).\n", cntDir);
    printf("\t%d \tfifo(s).\n", cntFifo);
    printf("\t%d \tsocket(s).\n", cntSock);
    printf("\t%d \tslink(s).\n", cntSlink);
    printf("\t%d \tchar dev(s).\n", cntCharD);
    printf("\t%d \tblock dev(s).\n", cntBlockD);
    return 1;
}