/**
 * Lab 5b -- Tarx
 * COSC360
 * Joey Lemon
 * Mar 1, 2020
 * 
 * This program builds a directory from a tar file, similar to "tar xf"
 */

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <utime.h>
#include "jrb.h"
#include "dllist.h"

// A structure to hold information about a file
typedef struct TFile {
	int name_length, mode;
    long inode, mtime, size;
    char filename[256];
    struct TFile *link;
    unsigned char* contents;
} TFile;

/**
 * Reads the next file from the tarc file given via stdin
 * @param tfile A pointer to a tfile structure
 * @param inodes A JRB of inodes to keep track of links
 * @return int 0 if the file was read successfully, -1 if we've reached EOF
 */
int read_next_file(TFile *tfile, JRB inodes) {
    // Read name length
    int name_length;

    // If we couldn't read name length, we know we've reached EOF
    if (fread(&name_length, sizeof(int), 1, stdin) != 1) return -1;

    tfile->name_length = name_length;

    // Ensure name length is positive so it can be malloced
    if (name_length <= 0) {
        fprintf(stderr, "Bad tarc file. File name size is %d, which I can't malloc.\n", name_length);
        exit(1);
    }

    // Read filename
    char filename[256];
    if (fread(&filename, sizeof(char), name_length, stdin) != name_length) {
        fprintf(stderr, "Bad tarc file.  Couldn't read name\n");
        exit(1);
    }

    // Add null terminator since tarc leaves it out
    filename[name_length] = '\0';
    strcpy(tfile->filename, filename);

    // Read inode
    long inode;
    if (fread(&inode, sizeof(long), 1, stdin) != 1) {
        fprintf(stderr, "Bad tarc file for %s.  Couldn't read inode\n", tfile->filename);
        exit(1);
    }
    tfile->inode = inode;

    // If inode has already been processed, set the link pointer and stop here
    JRB inode_jrb = jrb_find_int(inodes, inode);
    if (inode_jrb != NULL) {
        tfile->link = (TFile*) jrb_val(inode_jrb).v;
        return 0;
    }

    // Since inode didn't exist, there is no link
    tfile->link = NULL;
    jrb_insert_int(inodes, inode, new_jval_v(tfile));

    // Read mode
    int mode;
    if (fread(&mode, sizeof(int), 1, stdin) != 1) {
        fprintf(stderr, "Bad tarc file for %s.  Couldn't read mode\n", tfile->filename);
        exit(1);
    }
    tfile->mode = mode;

    // Read modification time
    long mtime;
    if (fread(&mtime, sizeof(long), 1, stdin) != 1) {
        fprintf(stderr, "Bad tarc file for %s.  Couldn't read mtime\n", tfile->filename);
        exit(1);
    }
    tfile->mtime = mtime;

    // If the file is a directory, stop here
    if(S_ISDIR(mode)) return 0;

    // Read file size
    long size;
    if (fread(&size, sizeof(long), 1, stdin) != 1) {
        fprintf(stderr, "Bad tarc file for %s.  Couldn't read size\n", tfile->filename);
        exit(1);
    }
    tfile->size = size;

    // Prevent bus error if size is negative or really large
    if (size <= 0 || size >= 100000000) {
        fprintf(stderr, "Bad tarc file for %s.  Can't read file of size %ld\n", tfile->filename, size);
        exit(1);
    }

    // Read file contents
    unsigned char buf[size];
    if (fread(&buf, sizeof(unsigned char), size, stdin) != size) {
        fprintf(stderr, "Bad tarc file for %s.  Couldn't read contents\n", tfile->filename);
        exit(1);
    }
    tfile->contents = (unsigned char*) malloc(sizeof(unsigned char) * size);

    memcpy(tfile->contents, buf, size);
}

int main() {
    JRB inodes = make_jrb();
    Dllist files = new_dllist();
    Dllist dirs = new_dllist();

    // Read all files from the tarc file and append them to the lists
    TFile *tfile;
    while (1) {
        tfile = (TFile*) malloc(sizeof(TFile));

        // If read_next_file returns -1, we've reached EOF
        if (read_next_file(tfile, inodes) == -1) {
            free(tfile);
            break;
        }

        // Append the file to the correct list depending on if its a directory
        if(S_ISDIR(tfile->mode))
            dll_append(dirs, new_jval_v(tfile));
        else
            dll_append(files, new_jval_v(tfile));
    }

    // Create all directories with writable modes
    // Do this before creating files to ensure the directories exist
    Dllist tmp;
    dll_traverse(tmp, dirs){
        TFile* tfile = (TFile*) tmp->val.v;
        mkdir(tfile->filename, S_IRWXU | S_IRWXG | S_IRWXO);
	}

    // Create all files and links
    dll_traverse(tmp, files){
        TFile* tfile = (TFile*) tmp->val.v;

        // Create a hard link if the file already had an inode
        if (tfile->link != NULL) {
            link(tfile->link->filename, tfile->filename);
            continue;
        }

        // Write the file contents
        FILE *file = fopen(tfile->filename, "wb");
        fwrite(tfile->contents, sizeof(unsigned char), tfile->size, file);
        fclose(file);

        // Set the file mode
        chmod(tfile->filename, tfile->mode);
        
        // Set the file modification time
        struct utimbuf times;
        times.modtime = tfile->mtime;
        times.actime = tfile->mtime;
        utime(tfile->filename, &times);
    }

    // Free file structs since we will no longer need them
    dll_traverse(tmp, files){
        TFile* tfile = (TFile*) tmp->val.v;
        free(tfile->contents);
        free(tfile);
    }

    // Set directory modes properly after writing all files and free dir structs
    // We set the modes after writing files to ensure we have writing permission
    dll_traverse(tmp, dirs){
        TFile* tfile = (TFile*) tmp->val.v;
        chmod(tfile->filename, tfile->mode);
        
        // Set the dir modification time since we just edited it with chmod
        struct utimbuf times;
        times.modtime = tfile->mtime;
        times.actime = tfile->mtime;
        utime(tfile->filename, &times);

        free(tfile);
    }

    free_dllist(files);
    free_dllist(dirs);
    jrb_free_tree(inodes);
}