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
 * @return int The exit status of the gcc command
 */
int gcc(char* compileType, Dllist flags, char* args);

/**
 * Compile all .o files for the list of .c files using gcc
 * 
 * A .o file is only compiled if:
 *    - There is no .o file corresponding to the .c file
 *    - There is a .o file corresponding to the .c file, but the .c file is more recent
 *    - There is a .o file corresponding to the .c file, and any of the .h files 
 *          are more recent than the .o file
 * 
 * @param Cfiles The list of .c files
 * @param flags The list of flag strings
 * @param header_time The most recent modify time for any .h file
 * @return int The number of files that were compiled. If no files were compiled, 
 *              it returns the time of the last .o modification
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
            if (executable != NULL) {
                fprintf(stderr, "fmakefile (%d) cannot have more than one E line\n", is->line);
                free_memory();
                exit(1);
            }

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

    struct stat statbuf;
    int executable_exists = stat(executable, &statbuf) != -1;

    // If no files were compiled, num_compiled will be the time of the last .o modification
    int num_compiled = compile_object_code(Cfiles, flags, last_header_change);

    // Compile executable if any .c files were compiled, or if the executable 
    // doesn't exist, or if there's a newer .o file
    if ( (num_compiled > 0 && num_compiled < 1000000) || !executable_exists || statbuf.st_mtime < num_compiled) {
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

MFile* insert_file(Dllist list, char* fileName) {
    // Allocate a new MFile
    MFile *file = (MFile *) malloc(sizeof(MFile));
    file->name = strdup(fileName);
    file->objectFile = NULL;

    struct stat statbuf;
    if (stat(fileName, &statbuf) != -1) {
        file->time = statbuf.st_mtime;
    } else {
        file->time = 0;
    }

    dll_append(list, new_jval_v((void *)file));
    return file;
}

MFile* mfile_add_object_file(MFile* mfile) {
    // Remove the .c extension from the name
    char* fileName = strdup(mfile->name);
    char* baseName = strtok(fileName, ".");

    // Build a new name with a .o extension
    char ofileName[strlen(baseName) + 2];
    strcpy(ofileName, baseName);
    strcat(ofileName, ".o");

    // We had to strdup fileName since strtok modifies the pointer, so we have to
    // to free fileName manually
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

    // All .c files need a corresponding .o file
    mfile_add_object_file(cfile);

    return cfile;
}

void free_file_list(Dllist list) {
    Dllist tmp;
    MFile *m;

    // Traverse all elements and free their specific fields that were
    // allocated using malloc
    dll_traverse(tmp, list) {
        m = (MFile *)tmp->val.v;

        // Don't forget to free the .o file here, since there isn't a
        // separate list of .o files
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

int gcc(char* compileType, Dllist flags, char* args) {
    char command[100];
    Dllist tmp;

    // Allocate a new string to hold our flags
    char* flagList = (char*) malloc(sizeof(char) * 100);
    flagList[0] = '\0';

    // Concatenate all flags into one string
    dll_traverse(tmp, flags) {
        strcat(flagList, tmp->val.s);
        strcat(flagList, " ");
    }

    // Format the string using our arguments
    sprintf(command, "gcc %s %s%s\n", compileType, flagList, args);

    // Perform the command and print it to stdout
    int result = system(command);
    printf(command);

    free(flagList);

    return result;
}

int compile_object_code(Dllist Cfiles, Dllist flags, int header_time) {
    int last_object_change = 0;
    int num_compiled = 0;

    Dllist tmp;
    MFile *m;

    // Compile all .c files that require a new .o file
    dll_traverse(tmp, Cfiles) {
        m = (MFile *)tmp->val.v;

        // If the file doesn't exist, it won't have a modified time
        if (m->time == 0) {
            fprintf(stderr, "fmakefile: %s: No such file or directory\n", m->name);
            exit(1);
        }

        // If the .o doesn't exist, or it has been modified, or if any .h files have been modified,
        // the recompile the .c file
        if (m->objectFile == NULL 
            || m->objectFile->time < m->time 
            || m->objectFile->time < header_time) {

            int result = gcc("-c", flags, m->name);
            if (result != 0) {
                fprintf(stderr, "Command failed.  Exiting\n");
                exit(1);
            }

            // Update the pointer to the object file if a new one was created
            if (m->objectFile == NULL)
                mfile_add_object_file(m);

            num_compiled++;
        }

        // Keep track of the time of the latest .o modification
        if (m->objectFile->time > last_object_change)
            last_object_change = m->objectFile->time;
    }

    // If no files were compiled, return the time of the last .o modification
    if (num_compiled == 0)
        return last_object_change;
    
    return num_compiled;
}

void compile_executable(Dllist Cfiles, Dllist Lfiles, Dllist flags, char* executable) {
    Dllist tmp;
    MFile *m;
    
    // Build a string for gcc's output file name
    char* compileType = (char*) malloc(sizeof(char) * (strlen(executable) + 4));
    strcpy(compileType, "-o ");
    strcat(compileType, executable);

    // Build a list of arguments for gcc
    char* argList = (char*) malloc(sizeof(char) * 200);
    argList[0] = '\0';

    // Concatenate all .o file names to the list of arguments
    dll_traverse(tmp, Cfiles) {
        m = (MFile *)tmp->val.v;
        strcat(argList, m->objectFile->name);
        strcat(argList, " ");
    }

    // Concatenate all library file names to the list of arguments
    dll_traverse(tmp, Lfiles) {
        strcat(argList, tmp->val.s);
        strcat(argList, " ");
    }

    // Remove the extra space at the end of the arguments list
    argList[strlen(argList) - 1] = '\0';

    int result = gcc(compileType, flags, argList);
    if (result != 0) {
        fprintf(stderr, "Command failed.  Fakemake exiting\n");
        exit(1);
    }

    free(compileType);
    free(argList);
}