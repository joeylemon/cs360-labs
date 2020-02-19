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

int print_file(char* longname, char* shortname, JRB inodes) {
    struct stat statbuf;
    if (stat(longname, &statbuf) == -1) {
        fprintf(stderr, "%s: No such file or directory\n", longname);
        return 1;
    } else if (S_ISLNK(statbuf.st_mode)) {
        return 1;
    }

    printf("0x%08x %-20s 0x%016x ", strlen(shortname), shortname, statbuf.st_ino);

    if (jrb_find_int(inodes, statbuf.st_ino) != NULL)
        return 0;
    jrb_insert_int(inodes, statbuf.st_ino, new_jval_s(longname));

    printf("0x%08x 0x%016x ", (int)statbuf.st_mode, statbuf.st_mtime);

    if(S_ISDIR(statbuf.st_mode)){
        return 0;
    }

    printf("0x%016x bytes", statbuf.st_size);
    return 0;
}

void iterate_dir(char* fullpath, char* shortpath, int root, JRB inodes) {
    struct dirent *dp;
    DIR *dfd;

    if ((dfd = opendir(fullpath)) == NULL) {
        fprintf(stderr, "%s: No such file or directory\n", fullpath);
        return;
    }

    char longname[256];
    char shortname[256];
    while ((dp = readdir(dfd)) != NULL) {
        /* Skip ./ and ../ */
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        sprintf(longname, "%s/%s", fullpath, dp->d_name);
        sprintf(shortname, "%s/%s", shortpath, dp->d_name);

        print_file(longname, shortname, inodes);
        printf("\n");

        struct stat statbuf;
        if (stat(longname, &statbuf) == -1)
            continue;

        if(S_ISDIR(statbuf.st_mode)) {
            char dir_name[256];
            strcpy(dir_name, shortname);
            iterate_dir(longname, dir_name, 0, inodes);
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: tarc directory\n");
        exit(1);
    }

    printf("%-10s %-20s %-18s %-10s %-18s %-18s\n", "name size", "name", "inode", "mode", "mod time", "file size");

    JRB inodes = make_jrb();
    char dir_suffix[256];
    strcpy(dir_suffix, get_dirname(strdup(argv[1])));

    if (print_file(argv[1], dir_suffix, inodes) != 0)
        exit(1);
    printf("\n");
    
    iterate_dir(argv[1], dir_suffix, 1, inodes);
}