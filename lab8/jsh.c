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

#define streq(str1, str2) strcmp(str1, str2) == 0

typedef struct cmd {
    char* args[100];
    int argc;
    int run_in_background;
    int completed;
    char* redirect_stdout;
    char* redirect_append_stdout;
    char* redirect_stdin;
    struct cmd *pipe_output;
} Command;

/**
 * Check if a string is a token for redirection
 * 
 * @param str The string
 * @return int 1 if the string is a token, 0 if not
 */
int is_token(char* str) {
    return strcmp(str, ">") == 0 || 
           strcmp(str, "<") == 0 || 
           strcmp(str, ">>") == 0;
}

/**
 * Scan the next redirect for the given command
 * 
 * @param is The input struct
 * @param current_index The current index into the field array
 * @param cmd The command to scan into
 * @param token The token to match against
 * @return int 1 if a redirection was scanned, 0 if not
 */
int scan_next_redirect(IS is, int current_index, Command* cmd, char* token) {
    if (strcmp(token, "<") == 0) {
        cmd->redirect_stdin = strdup(is->fields[current_index + 1]);
    } else if (strcmp(token, ">") == 0) {
        cmd->redirect_stdout = strdup(is->fields[current_index + 1]);
    } else if (strcmp(token, ">>") == 0) {
        cmd->redirect_append_stdout = strdup(is->fields[current_index + 1]);
    } else {
        return 0;
    }

    return 1;
}

/**
 * Scan all redirects for the given command
 * 
 * @param is The input struct
 * @param current_index The current index into the field array
 * @param cmd The command to scan into
 * @return int The index where the next command's fields begin
 */
int scan_redirects(IS is, int current_index, Command* cmd) {
    int i = current_index;
    int result = 1;

    while (result) {
        result = scan_next_redirect(is, i, cmd, is->fields[i]);
        if (result) i += 2;
    }

    return i;
}

/**
 * Scan the next command
 * 
 * @param is The input struct
 * @param current_index The current index into the field array
 * @param cmd The command to scan into
 * @return int -1 if we've reached the end of all commands, -2 if nothing was scanned, otherwise, the index
 *             where the next command's fields begin
 */
int scan_next_cmd(IS is, int current_index, Command* cmd) {
    int i;

    for (i = current_index; i < is->NF; i++) {
        char* field = is->fields[i];

        // Scan command up until next pipe
        if (strcmp(field, "|") == 0) {
            return i + 1;

        // Scan to end of all commands
        } else if (strcmp(field, ";") == 0) {
            return -1;

        // Scan background token
        } else if (strcmp(field, "&") == 0) {
            cmd->run_in_background = 1;
            return -1;

        // Scan all redirects for the current command
        } else if (is_token(field)) {
            return scan_redirects(is, i, cmd);

        // Add field to current command's arguments
        } else {
            cmd->args[cmd->argc++] = strdup(field);
        }
    }

    return -2;
}

/**
 * Scan the entire line of input into commands
 * 
 * @param is The input struct
 * @return Command* The first command to execute
 */
Command* get_cmd(IS is) {
    int i = 0;

    Command* head = NULL;
    Command* cmd = NULL;

    // Add semicolon at end of input to represent end of all commands
    is->fields[is->NF] = ";";
    is->NF++;

    while (i >= 0 && i < is->NF) {
        Command* new_cmd = (Command*) malloc(sizeof(Command));
        i = scan_next_cmd(is, i, new_cmd);

        // Keep track of the first command
        if (head == NULL)
            head = new_cmd;

        // If nothing was scanned
        if (new_cmd->argc == 0) {
            free(new_cmd);
            break;
        }

        if (cmd != NULL)
            cmd->pipe_output = new_cmd;

        cmd = new_cmd;

        // If nothing was scanned
        if (i == -2) {
            free(new_cmd);
            break;
        }
    }

    return head;
}

void print_command(Command* cmd) {
    int i;

    printf("\nArgs:\n");
    for (i = 0; i < cmd->argc; i++) {
        printf("  [%d]: %s\n", i, cmd->args[i]);
    }
    printf("\n");

    printf("%-25s%d\n", "Run in background:", cmd->run_in_background);

    printf("%-25s", "Redirect stdin:");
    if (cmd->redirect_stdin != NULL) printf("%s", cmd->redirect_stdin);
    else printf("none");
    printf("\n");

    printf("%-25s", "Redirect stdout:");
    if (cmd->redirect_stdout != NULL) printf("%s", cmd->redirect_stdout);
    else printf("none");
    printf("\n");

    printf("%-25s", "Redirect append stdout:");
    if (cmd->redirect_append_stdout != NULL) printf("%s", cmd->redirect_append_stdout);
    else printf("none");
    printf("\n");

    printf("%-25s", "Pipe command:");
    if (cmd->pipe_output == NULL) printf("none\n");
    else {
        printf("\n");
        print_command(cmd->pipe_output);
    }
}

void run_command(Command* cmd) {
    int s1, s2;
    int pipefd[2];

    // Create pipe if necessary
    if (cmd->pipe_output != NULL) {
        if (pipe(pipefd) < 0) {
            perror("jsh: pipe");
            exit(1);
        }
    }

    // Child process
    if (fork() == 0) {
        if (cmd->redirect_stdin != NULL) {
            int fd = open(cmd->redirect_stdin, O_RDONLY);
            if (fd < 0) {
                perror("jsh: redirect stdin");
                exit(1);
            }

            if (dup2(fd, 0) != 0) {
                perror("jsh: dup2(fd, 0)");
                exit(1);
            }
            close(fd);
        }
        
        if (cmd->redirect_stdout != NULL) {
            int fd = open(cmd->redirect_stdout, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            if (fd < 0) {
                perror("jsh: redirect stdin");
                exit(1);
            }

            if (dup2(fd, 1) != 1) {
                perror("jsh: dup2(fd, 1)");
                exit(1);
            }
            close(fd);
        }
        
        if (cmd->redirect_append_stdout != NULL) {
            int fd = open(cmd->redirect_append_stdout, O_WRONLY | O_APPEND | O_CREAT, 0644);
            if (fd < 0) {
                perror("jsh: redirect stdin");
                exit(1);
            }

            if (dup2(fd, 1) != 1) {
                perror("jsh: dup2(fd, 1)");
                exit(1);
            }
            close(fd);
        }
        
        if (cmd->pipe_output != NULL) {
            if (dup2(pipefd[1], 1) != 1) {
                perror("jsh: dup2(pipefd[1])");
                exit(1);
            }

            close(pipefd[1]);
            close(pipefd[0]);
        }

        execvp(cmd->args[0], cmd->args);
        perror(cmd->args[0]);
        exit(1);

    // Second child (pipe)
    } else if (cmd->pipe_output != NULL && fork() == 0) {
        if (dup2(pipefd[0], 0) != 0) {   
            perror("jsh: dup2(pipefd[0])");
            exit(1);
        }

        close(pipefd[1]);
        close(pipefd[0]);

        run_command(cmd->pipe_output);

    // Parent process
    } else {
        if (cmd->pipe_output != NULL) {
            close(pipefd[1]);
            close(pipefd[0]);
            wait(&s2);
            cmd->pipe_output->completed = 1;
        }

        if (!cmd->run_in_background)
            wait(&s1);
            cmd->completed = 1;
    }
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

    Command* cmd = NULL;
    IS is = new_inputstruct(NULL);
    while (1) {
        // End the program if this is a forked version
        if (cmd != NULL && !cmd->completed) break;

        // Print the prompt
        printf("%s", prompt);

        // Get the command from the prompt
        if (get_line(is) < 0) break;

        // Skip empty commands
        if (is->NF == 0) continue;

        cmd = get_cmd(is);
        
        //print_command(cmd);
        run_command(cmd);
    }

    jettison_inputstruct(is);

    return 0;
}