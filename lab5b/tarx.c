/**
 * Lab 5a -- Tarc
 * COSC360
 * Joey Lemon
 * Feb 23, 2020
 * 
 * This program builds a directory from a tar file, similar to "tar xf"
 */

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "jrb.h"
#include "dllist.h"

int read_next_file(JRB inodes) {
    int name_length;
    if (fread(&name_length, sizeof(int), 1, stdin) != 1)
        return -1;
    printf("name_length: %d\n", name_length);

    char filename[255];
    if (fread(&filename, sizeof(char), name_length, stdin) != name_length)
        return -1;
    printf("filename: %s\n", filename);

    long inode;
    if (fread(&inode, sizeof(long), 1, stdin) != 1)
        return -1;
    printf("inode: %d\n", inode);

    /* If inode has already been processed, stop here */
    if (jrb_find_int(inodes, inode) != NULL)
        return 0;

    jrb_insert_int(inodes, inode, new_jval_s(filename));

    int mode;
    long mtime;
    if (fread(&mode, sizeof(int), 1, stdin) != 1)
        return -1;
    printf("mode: %d\n", mode);
    if (fread(&mtime, sizeof(long), 1, stdin) != 1)
        return -1;
    printf("mtime: %d\n", mtime);

    if(S_ISDIR(mode)){
        /* directory stuff (dllist) */
        return 0;
    } else {
        long size;
        if (fread(&size, sizeof(long), 1, stdout) != 1)
            return -1;
        printf("size: %d\n", size);

        unsigned char buf[size];
        if (fread(&buf, sizeof(unsigned char), size, stdout) != size)
            return -1;
    }
}

int main() {
    JRB inodes = make_jrb();

    while (read_next_file(inodes) != -1);

    jrb_free_tree(inodes);
}