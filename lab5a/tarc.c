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
#include "dllist.h"

typedef struct Directory {
	char longname[256];
    char shortname[256];
} Directory;

/**
 * Get the suffix of a directory name
 * E.g.: "././d1/../d1" returns "d1"
 * 
 * @param dir The name of the directory
 * @return char* The suffix of the directory
 */
char* get_dir_suffix(char* dir) {
    char* token = strtok(dir, "/");
    char* last;

    /* Continuously move forward by the '/' token until we've reached the end */
    while( token != NULL ) {
        last = token;
        token = strtok(NULL, "/");
    }

    /* The last string past the token is the suffix */
    return last;
}

/**
 * Print a file's information bytes to stdout
 * 
 * @param longname The full path to the file (e.g. "/home/jlemon3/labs/lab5/d1/f1.txt")
 * @param shortname The shortened path to the file (e.g. "d1/f1.txt")
 * @param inodes The tree of inodes
 * @return int 1 if an error occurred, 0 otherwise
 */
int print_file(char* longname, char* shortname, JRB inodes) {
    /* Get file attributes */
    struct stat statbuf;
    if (stat(longname, &statbuf) == -1) {
        fprintf(stderr, "%s: No such file or directory\n", longname);
        exit(1);
    } else if (S_ISLNK(statbuf.st_mode)) {
        /* We want to skip soft links */
        return 1;
    }

    /* Write name information */
    int name_length = strlen(shortname);
    fwrite(&name_length, sizeof(int), 1, stdout);
    fwrite(shortname, sizeof(char), name_length, stdout);

    /* Write inode */
    fwrite(&statbuf.st_ino, sizeof(long), 1, stdout);

    /* If the inode was already processed, don't print mode/mod time/file info */
    if (jrb_find_int(inodes, statbuf.st_ino) != NULL)
        return 0;

    jrb_insert_int(inodes, statbuf.st_ino, new_jval_s(longname));

    /* Write mode and modification time */
    fwrite(&statbuf.st_mode, sizeof(int), 1, stdout);
    fwrite(&statbuf.st_mtime, sizeof(long), 1, stdout);

    /* If the file is a directory, don't print file size/file bytes */
    if(S_ISDIR(statbuf.st_mode)){
        return 0;
    }

    /* Create a buffer to hold the file's contents */
    unsigned char buf[statbuf.st_size];
    FILE *file = fopen(longname, "r");
    if (file == NULL) {
        exit(1);
    }

    /* Read the file's contents into the buffer */
    fread(&buf, sizeof(unsigned char), statbuf.st_size, file);
    fclose(file);

    /* Write the file's size and contents */
    fwrite(&statbuf.st_size, sizeof(long), 1, stdout);
    fwrite(&buf, sizeof(unsigned char), statbuf.st_size, stdout);


    return 0;
}

/**
 * Parse all of the files in the given directory and recursively parse 
 * all subdirectories
 * 
 * @param directory The directory struct with path names
 * @param inodes The tree of inodes
 */
void iterate_dir(Directory* directory, JRB inodes) {
    struct dirent *dp;
    DIR *dfd = opendir(directory->longname);

    if (dfd == NULL) {
        fprintf(stderr, "%s: No such file or directory\n", directory->longname);
        exit(1);
    }

    Dllist dirs = new_dllist();

    /* Keep track of both full file path and shortened path */
    char longname[256];
    char shortname[256];

    while ((dp = readdir(dfd)) != NULL) {
        /* Skip ./ and ../ */
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        /* Build the file path using the current directory */
        sprintf(longname, "%s/%s", directory->longname, dp->d_name);
        sprintf(shortname, "%s/%s", directory->shortname, dp->d_name);

        /* Print the file information to stdout */
        print_file(longname, shortname, inodes);

        struct stat statbuf;
        if (stat(longname, &statbuf) == -1)
            continue;

        /* If the file is a directory, add to dir list to iterate */
        if(S_ISDIR(statbuf.st_mode)) {
            Directory *dir = (Directory*) malloc(sizeof(Directory));
            /* Continuously append directories onto the path */
            strcpy(dir->shortname, shortname);
            strcpy(dir->longname, longname);
            dll_append(dirs, new_jval_v(dir));
        }
    }

    closedir(dfd);

    /* Recursively iterate all the directories in the current directory */
    Dllist tmp;
    dll_traverse(tmp, dirs){
        Directory* dir = (Directory*) tmp->val.v;
        iterate_dir(dir, inodes);
		free(dir);
	}

    free_dllist(dirs);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: tarc directory\n");
        exit(1);
    }

    char* dir = strdup(argv[1]);
    char dir_suffix[256];
    strcpy(dir_suffix, get_dir_suffix(dir));

    JRB inodes = make_jrb();

    /* If the given directory doesn't exist, exit immediately */
    if (print_file(argv[1], dir_suffix, inodes) != 0)
        exit(1);

    /* Create a struct to represent the given directory */
    Directory *directory = (Directory*) malloc(sizeof(Directory));
    strcpy(directory->longname, argv[1]);
    strcpy(directory->shortname, dir_suffix);

    /* Begin iterating at the given directory, and recursively iterate subdirectories */
    iterate_dir(directory, inodes);

    free(directory);
    free(dir);
    jrb_free_tree(inodes);
}