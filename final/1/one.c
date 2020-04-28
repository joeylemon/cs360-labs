// Joey Lemon
// Question 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "jrb.h"

int hlunq(char *d, JRB inodes) {
    int num_uniq = 0;

    JRB dirs = make_jrb();
    DIR *dfd = opendir(d);

    struct dirent *dp;
    while ((dp = readdir(dfd)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        char filename[256];
        sprintf(filename, "%s/%s", d, dp->d_name);

        struct stat statbuf;
        if (stat(filename, &statbuf) == -1) continue;

        if (jrb_find_int(inodes, statbuf.st_ino) != NULL) continue;

        if(S_ISDIR(statbuf.st_mode)) {
            jrb_insert_str(dirs, filename, new_jval_s(strdup(filename)));
            continue;
        }

        jrb_insert_int(inodes, statbuf.st_ino, new_jval_v());
        num_uniq++;
    }

    closedir(dfd);

    JRB tmp;
    jrb_traverse(tmp, dirs) {
        char* dir = tmp->val.s;
        num_uniq += hlunq(dir, inodes);
        free(dir);
    }

    jrb_free_tree(dirs);

    return num_uniq;
}

// Joey Lemon
// Question 1
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s directory\n", argv[0]);
        exit(1);
    }

    JRB inodes = make_jrb();
    printf("Number of unique files in %s: %d\n", argv[1], hlunq(argv[1], inodes));
}