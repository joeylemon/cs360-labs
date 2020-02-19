/**
 * Lab 5a -- Tarc
 * COSC360
 * Joey Lemon
 * Feb 23, 2020
 * 
 * This program bundles a directory, similar to "tar cf"
 */

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "jrb.h"

// ../lab5/../lab5/d1 => d1
char* get_dirname(char* dir) {
    char* token = strtok(dir, "/");
    char* last;

    while( token != NULL ) {
        last = token;
        token = strtok(NULL, "/");
    }

    return last;
}

void print_file(char* curfile, JRB inodes) {
    struct stat statbuf;
    if (stat(curfile, &statbuf) == -1) {
        fprintf(stderr, "%s: No such file or directory\n", curfile);
        return;
    } else if (S_ISLNK(statbuf.st_mode)) {
        return;
    }

    printf("0x%08x %-20s 0x%016x ", strlen(curfile), curfile, statbuf.st_ino);

    if (jrb_find_int(inodes, statbuf.st_ino) != NULL)
        return;
    jrb_insert_int(inodes, statbuf.st_ino, new_jval_s(curfile));

    printf("0x%08x 0x%016x ", (int)statbuf.st_mode, statbuf.st_mtime);

    if(S_ISDIR(statbuf.st_mode)){
        return;
    }

    printf("0x%016x bytes", statbuf.st_size);
}

void iterate_dir(char* dir, int root, JRB inodes) {
    struct dirent *dp;
    DIR *dfd;

    if ((dfd = opendir(dir)) == NULL) {
        fprintf(stderr, "%s: No such file or directory\n", dir);
        return;
    }

    char curfile[256];
    while ((dp = readdir(dfd)) != NULL) {
        /* Skip ./ and ../ */
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        sprintf(curfile, "%s/%s", dir, dp->d_name);

        print_file(curfile, inodes);
        printf("\n");

        struct stat statbuf;
        if (stat(curfile, &statbuf) == -1)
            continue;

        if(S_ISDIR(statbuf.st_mode))
            iterate_dir(curfile, 0, inodes);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: tarc directory\n");
        exit(1);
    }

    printf("%-10s %-20s %-18s %-10s %-18s %-18s\n", "name size", "name", "inode", "mode", "mod time", "file size");

    JRB inodes = make_jrb();
    print_file(argv[1], inodes);
    printf("\n");
    iterate_dir(argv[1], 1, inodes);
}