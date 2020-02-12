#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "fields.h"
#include "dllist.h"

#define free_memory() {\
    jettison_inputstruct(is);\
    free_file_list(Cfiles);\
    free_file_list(Hfiles);\
    free_string_list(Lfiles);\
    free_string_list(flags);\
    if (executable != NULL) free(executable);\
}

/**
 * A structure to hold information about a file
 * If it is a .c file, "objectFile" will point to its .o file
 */
typedef struct mfile {
    char *name;
    int time;
    struct mfile *objectFile;
} MFile;

/**
 * Check if two strings are equal
 * 
 * @return int true if equal, false if not
 */
int equal(char* str1, char* str2);

/**
 * Check if a file exists
 * 
 * @param fileName The name of the file
 * @return int true if equal, false if not
 */
int file_exists(char* fileName);

/**
 * Performs stat on a file and inserts a new MFile into the list
 * 
 * @param list The list to insert into
 * @param fileName The name of the file
 * @return A pointer to the file that was inserted
 */
MFile* insert_file(Dllist list, char* fileName);

/**
 * Extracts the .c file's base name ("mysort.c" has a basename of "mysort")
 * and creates an MFile for its corresponding .o file
 * 
 * @param mfile The MFile for the .c file
 * @return MFile* A pointer to the new MFile
 */
MFile* mfile_add_object_file(MFile* mfile);

/**
 * Insert a .c file into the list, and then set the .c file's object file pointer
 * if it has an existing .o file
 * 
 * @param Cfiles The list of .c files
 * @param fileName The name of the new .c file
 * @return MFile* A pointer to the new MFile
 */
MFile* insert_c_file(Dllist Cfiles, char* fileName);

/**
 * Free all MFiles in the list
 * 
 * @param list The list of MFiles
 */
void free_file_list(Dllist list);

/**
 * Free all strings in the list
 * 
 * @param list The list of strings
 */
void free_string_list(Dllist list);

/**
 * Call "gcc" in the command-line with the given compile type ("-o" or "-c"), the given
 * flags, and the given arguments
 * 
 * @param compileType The compile type ("-o" or "-c")
 * @param flags The list of flag strings
 * @param args A single string containing arguments (such as file names to compile)
 */
void gcc(char* compileType, Dllist flags, char* args);

/**
 * Compile all .o files for the list of .c files using gcc
 * 
 * A .o file is only compiled if:
 *    - There is no .o file corresponding to the .c file
 *    - There is a .o file corresponding to the .c file, but the .c file is more recent
 *    - There is a .o file corresponding to the .c file, and any of the .h files are more recent than the .o file
 * 
 * @param Cfiles The list of .c files
 * @param flags The list of flag strings
 * @param header_time The most recent modify time for any .h file
 * @return int The number of files that were compiled
 */
int compile_object_code(Dllist Cfiles, Dllist flags, int header_time);

/**
 * Compile the executable file using gcc
 * 
 * @param Cfiles The list of .c files
 * @param Lfiles The list of library file strings
 * @param flags The list of flag strings
 * @param executable The name of the executable
 */
void compile_executable(Dllist Cfiles, Dllist Lfiles, Dllist flags, char* executable);


int main(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "usage: fakemake [description-file]\n");
        exit(1);
    }

    // Default input file to "fmakefile", then see if user specified a different file
    char* inputfile = "fmakefile";
    if (argc == 2)
        inputfile = argv[1];

    IS is = new_inputstruct(inputfile);

    // If file does not exist, 'is' will be null
    if (is == NULL) {
        fprintf(stderr, "fakemake: %s: No such file or directory\n", inputfile);
        exit(1);
    }

    int i;
    char* executable = NULL;

    Dllist Cfiles = new_dllist();
    Dllist Hfiles = new_dllist();
    Dllist Lfiles = new_dllist();
    Dllist flags = new_dllist();

    while (get_line(is) >= 0) {
        // Skip useless lines
        if (is->NF <= 1) continue;

        // The keyword is the first word in the line
        char *keyword = is->fields[0];

        // C source files
        if (equal(keyword, "C")) {
            for (i = 1; i < is->NF; i++) {
                if (insert_c_file(Cfiles, is->fields[i]) == NULL) {
                    free_memory();
                    exit(1);
                }
            }

        // C header files
        } else if (equal(keyword, "H")) {
            for (i = 1; i < is->NF; i++)
                if (insert_file(Hfiles, is->fields[i]) == NULL) {
                    free_memory();
                    exit(1);
                }

        // Name of executable
        } else if (equal(keyword, "E")) {
            executable = strdup(is->fields[1]);

        // Compilation flags
        } else if (equal(keyword, "F")) {
            for (i = 1; i < is->NF; i++)
                dll_append(flags, new_jval_s(strdup(is->fields[i])));

        // Link files
        } else if (equal(keyword, "L")) {
            for (i = 1; i < is->NF; i++)
                dll_append(Lfiles, new_jval_s(strdup(is->fields[i])));
        }
    }

    // Make sure the executable was listed in the makefile
    if (executable == NULL) {
        fprintf(stderr, "No executable specified\n");
        free_memory();
        exit(1);
    }

    Dllist tmp;
    MFile *m;

    // Traverse the header files for the most recent one
    int last_header_change = 0;
    dll_traverse(tmp, Hfiles) {
        m = (MFile *)tmp->val.v;

        if (m->time > last_header_change)
            last_header_change = m->time;
    }

    int num_compiled = compile_object_code(Cfiles, flags, last_header_change);
    if (num_compiled > 0 || !file_exists(executable)) {
        compile_executable(Cfiles, Lfiles, flags, executable);
    } else {
        printf("%s up to date\n", executable);

        free_memory();
        exit(1);
    }

    free_memory();

    return 0;
}


int equal(char* str1, char* str2) {
    return strcmp(str1, str2) == 0;
}

int file_exists(char* fileName) {
    struct stat statbuf;
    return stat(fileName, &statbuf) != -1;
}

MFile* insert_file(Dllist list, char* fileName) {
    struct stat statbuf;
    if (stat(fileName, &statbuf) == -1) {
        // TODO: don't check this for Lfiles?
        fprintf(stderr, "fakemake: %s: No such file or directory\n", fileName);
        return NULL;
    }

    MFile *file = (MFile *) malloc(sizeof(MFile));
    file->name = strdup(fileName);
    file->time = statbuf.st_mtime;
    file->objectFile = NULL;

    dll_append(list, new_jval_v((void *)file));
    return file;
}

MFile* mfile_add_object_file(MFile* mfile) {
    // Remove the .c extension from the name
    char* fileName = strdup(mfile->name);
    char* baseName = strtok(fileName, ".");
    char ofileName[strlen(baseName) + 2];
    strcpy(ofileName, baseName);
    strcat(ofileName, ".o");
    free(fileName);

    // Update the object file pointer in the mfile if the .o file exists
    struct stat statbuf;
    if (stat(ofileName, &statbuf) != -1) {
        MFile *file = (MFile *) malloc(sizeof(MFile));
        file->name = strdup(ofileName);
        file->time = statbuf.st_mtime;
        mfile->objectFile = file;

        return file;
    }

    return NULL;
}

MFile* insert_c_file(Dllist Cfiles, char* fileName) {
    MFile* cfile = insert_file(Cfiles, fileName);
    if (cfile == NULL) return NULL;

    mfile_add_object_file(cfile);

    return cfile;
}

void free_file_list(Dllist list) {
    Dllist tmp;
    MFile *m;
    dll_traverse(tmp, list) {
        m = (MFile *)tmp->val.v;

        if (m->objectFile != NULL) {
            free(m->objectFile->name);
            free(m->objectFile);
        }

        free(m->name);
        free(m);
    }
    free_dllist(list);
}

void free_string_list(Dllist list) {
    Dllist tmp;
    dll_traverse(tmp, list) {
        free(tmp->val.s);
    }
    free_dllist(list);
}

void gcc(char* compileType, Dllist flags, char* args) {
    char command[100];

    Dllist tmp;
    char* flagList = (char*) malloc(sizeof(char) * 100);
    flagList[0] = '\0';
    dll_traverse(tmp, flags) {
        strcat(flagList, tmp->val.s);
        strcat(flagList, " ");
    }

    sprintf(command, "gcc %s %s%s\n", compileType, flagList, args);
    system(command);
    printf(command);

    free(flagList);
}

int compile_object_code(Dllist Cfiles, Dllist flags, int header_time) {
    int num_compiled = 0;

    Dllist tmp;
    MFile *m;

    // Compile all .c files that require a new .o file
    dll_traverse(tmp, Cfiles) {
        m = (MFile *)tmp->val.v;

        // If the .o doesn't exist, or it has been modified, or if any .h files have been modified,
        // the recompile the .c file
        if (m->objectFile == NULL 
            || m->objectFile->time < m->time 
            || m->objectFile->time < header_time) {
            gcc("-c", flags, m->name);

            // Update the pointer to the object file if a new one was created
            if (m->objectFile == NULL)
                mfile_add_object_file(m);

            num_compiled++;
        }
    }

    return num_compiled;
}

void compile_executable(Dllist Cfiles, Dllist Lfiles, Dllist flags, char* executable) {
    char* compileType = (char*) malloc(sizeof(char) * (strlen(executable) + 4));
    strcpy(compileType, "-o ");
    strcat(compileType, executable);

    // Build a list of arguments for gcc
    char* argList = (char*) malloc(sizeof(char) * 200);
    argList[0] = '\0';

    Dllist tmp;
    MFile *m;

    // Add all .o files to the list of arguments
    dll_traverse(tmp, Cfiles) {
        m = (MFile *)tmp->val.v;
        strcat(argList, m->objectFile->name);
        strcat(argList, " ");
    }

    // Add all library files to the list of arguments
    dll_traverse(tmp, Lfiles) {
        strcat(argList, tmp->val.s);
        strcat(argList, " ");
    }
    argList[strlen(argList) - 1] = '\0';

    gcc(compileType, flags, argList);

    free(compileType);
    free(argList);
}