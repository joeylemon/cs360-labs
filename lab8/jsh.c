/**
 * Lab 8 -- Jsh
 * COSC360
 * Joey Lemon
 * Apr 5, 2020
 * 
 * This program implements a primitive shell, similar to csh
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "fields.h"
#include "jrb.h"
#include "dllist.h"

typedef struct field {
    char** args;
    int argc;
    int run_in_background;
    struct field *redirect_stdout;
    struct field *redirect_append_stdout;
    struct field *redirect_stdin;
} Field;

// **fields (*fields[1000]):
//     char* fields[0] = "cat"
//     char* fields[1] = "jsh.c"
//     char* fields[2] = ">"
//     char* fields[3] = "test.txt"

Field* split_fields(int NF, char** fields) {
    Field* head = NULL;
    Field* last_field = NULL;
    char last_token[3];

    int i = 0;
    int ii = 0;

    for (i = 0; i < NF; i++) {
        // Find the next special token
        if (strcmp(fields[i], ">") != 0 && 
            strcmp(fields[i], "<") != 0 && 
            strcmp(fields[i], ">>") != 0 &&
            strcmp(fields[i], "&") != 0 &&
            i + 1 != NF) continue;
        
        if (strcmp(fields[i], "&") != 0 && i + 1 == NF) i++;

        // Allocate a new field
        Field* f = (Field*) malloc(sizeof(Field));
        f->args = malloc(sizeof(char*) * (i-ii));
        f->argc = i-ii;

        // Extract the arguments
        int k = 0;
        int j;
        for (j = ii; j < i; j++) {
            f->args[k++] = strdup(fields[j]);
        }

        if (strcmp(last_token, ">") == 0) {
            last_field->redirect_stdout = f;
        } else if (strcmp(last_token, "<") == 0) {
            last_field->redirect_stdin = f;
        } else if (strcmp(last_token, ">>") == 0) {
            last_field->redirect_append_stdout = f;
        } else if (i != NF && strcmp(fields[i], "&") == 0) {
            f->run_in_background = 1;
        }

        if (head == NULL) head = f;
        last_field = f;

        if (i == NF) break;

        if (strcmp(fields[i], ">") == 0 || 
            strcmp(fields[i], "<") == 0 || 
            strcmp(fields[i], ">>") == 0) strcpy(last_token, fields[i]);

        // Update the last position (skipping the special token)
        ii = i + 1;
    }

    return head;
}

int main(int argc, char **argv, char **envp) {
    char prompt[100];

    // Determine which prompt to use
    switch (argc) {
    case 1:
        // Default to jsh prompt
        strcpy(prompt, "jsh: ");
        break;
    case 2:
        // Don't print a prompt if user specified '-'
        if (strcmp(argv[1], "-") == 0) {
            strcpy(prompt, "");
            break;
        }

        strcpy(prompt, argv[1]);
        break;
    default:
        // User specified improper arguments
        fprintf(stderr, "usage: %s [prompt]\n", argv[0]);
        exit(1);
    }

    int status;
    IS is = new_inputstruct(NULL);
    while (1) {
        printf("%s", prompt);

        // Get the command from the prompt
        if (get_line(is) < 0) break;

        // Skip empty commands
        if (is->NF == 0) continue;

        Field *f = split_fields(is->NF, is->fields);

        // Child process
        if (fork() == 0) {
            int fd_in, fd_out;

            if (f->redirect_stdin != NULL) {
                fd_in = open(f->redirect_stdin->args[0], O_RDONLY);
                if (fd_in < 0) {
                    perror("jsh: redirect stdin");
                    exit(1);
                }

                if (dup2(fd_in, 0) != 0) {
                    perror("jsh: dup2(fd_in, 0)");
                    exit(1);
                }
                close(fd_in);
            }

            if (f->redirect_stdout != NULL) {
                fd_out = open(f->redirect_stdout->args[0], O_WRONLY | O_TRUNC | O_CREAT, 0644);
                if (fd_out < 0) {
                    perror("jsh: redirect stdin");
                    exit(1);
                }

                if (dup2(fd_out, 1) != 1) {
                    perror("jsh: dup2(fd_out, 0)");
                    exit(1);
                }
                close(fd_out);
            }

            status = execvp(f->args[0], f->args);
            perror("fork");
            exit(1);

        // Parent process
        } else {
            if (!f->run_in_background)
                wait(&status);
        }
    }

    return 0;
}