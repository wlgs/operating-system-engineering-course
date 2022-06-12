#define _XOPEN_SOURCE 500

#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ftw.h>

// GLOBAL BECAUSE OF RECURSION
// THIS PROGRAM IS BASED ON nftw()

int cntFiles = 0;
int cntDir = -1;
int cntFifo = 0;
int cntSock = 0;
int cntSlink = 0;
int cntCharD = 0;
int cntBlockD = 0;

int printFileInfo(const char *filename, const struct stat *stats, int flags, struct FTW *ftw) {
    char *buffer = (char *) calloc(512, sizeof(char));
    char *res = realpath(filename, buffer);
    printf("\tPath: \t%s\n", res);
    printf("\tType: \t");
    if (S_ISREG(stats->st_mode)) {
        cntFiles += 1;
        printf("file\n");
    } else if (S_ISDIR(stats->st_mode)) {
        cntDir += 1;
        printf("dir\n");
    } else if (S_ISLNK(stats->st_mode)) {
        cntSlink += 1;
        printf("slink\n");
    } else if (S_ISCHR(stats->st_mode)) {
        cntCharD += 1;
        printf("char dev\n");
    } else if (S_ISBLK(stats->st_mode)) {
        printf("block dev\n");
        cntBlockD += 1;
    } else if (S_ISFIFO(stats->st_mode)) {
        cntFifo += 1;
        printf("fifo\n");
    } else if (S_ISSOCK(stats->st_mode)) {
        cntSock += 1;
        printf("sock\n");
    }
    printf("\tFile size: \t%ld\n", stats->st_size);
    printf("\tLast access: \t%s", ctime(&stats->st_atime));
    printf("\tLast modification: \t%s\n", ctime(&stats->st_mtime));
    printf("\tSymbolic links: \t%ld\n", stats->st_nlink);
    return 0;
}

int main(int argc, char **argv) {
    argv++;
    nftw(argv[0], printFileInfo, 2, FTW_PHYS);
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