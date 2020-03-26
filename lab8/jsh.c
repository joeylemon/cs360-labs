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
    char* redirect_stdout;
    char* redirect_append_stdout;
    char* redirect_stdin;
    struct cmd *pipe_output;
} Command;

// **fields (*fields[1000]):
//     char* fields[0] = "cat"
//     char* fields[1] = "jsh.c"
//     char* fields[2] = ">"
//     char* fields[3] = "test.txt"

//     char* fields[0] = "sleep"
//     char* fields[1] = "2"
//     char* fields[2] = "&"

//     char* fields[0] = "cat"
//     char* fields[1] = "jsh.c"
//     char* fields[2] = ">"
//     char* fields[3] = "test.txt"
//     char* fields[4] = "&&"
//     char* fields[5] = "ls"

int is_token(char* str) {
    return strcmp(str, ">") == 0 || 
           strcmp(str, "<") == 0 || 
           strcmp(str, ">>") == 0;
}

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

int scan_redirects(IS is, int current_index, Command* cmd) {
    int i = current_index;
    int result = 1;

    while (result) {
        result = scan_next_redirect(is, i, cmd, is->fields[i]);
        if (result) i += 2;
    }

    return i;
}

int scan_next_cmd(IS is, int current_index, Command* cmd) {
    int i;

    for (i = current_index; i < is->NF; i++) {
        char* field = is->fields[i];

        if (strcmp(field, "|") == 0) {
            return i + 1;
        } else if (strcmp(field, ";") == 0) {
            return -1;
        } else if (is_token(field)) {
            return scan_redirects(is, i, cmd);
        } else {
            cmd->args[cmd->argc++] = strdup(field);
        }
    }

    return -2;
}

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

        if (new_cmd->argc > 0) {
            if (cmd != NULL)
                cmd->pipe_output = new_cmd;

            cmd = new_cmd;
        } else {
            free(new_cmd);
        }

        // If nothing was scanned
        if (i == -2) {
            free(new_cmd);
            break;
        }
    }

    return head;
}



















/*
Command* extract_commands(IS is) {
    Command* head = NULL;
    Command* cmd = NULL;
    int i, j;
    int last_i = 0;

    for (i = 0; i < is->NF; i++) {
        // Check for ampersand to run command in background
        if (strcmp(is->fields[i], "&") == 0) {
            if (cmd != NULL) cmd->run_in_background = 1;

        // Check if we've encountered a token, or if we've reached the end
        } else if (is_token(is->fields[i]) || i + 1 == is->NF) {
            if (i + 1 == is->NF) i++;

            Command* new_cmd = (Command*) malloc(sizeof(Command));
            new_cmd->args = malloc(sizeof(char) * (i - last_i + 1));
            new_cmd->argc = i - last_i;

            // Keep track of the first command
            if (head == NULL)
                head = new_cmd;

            // Extract arguments
            int k = 0;
            for (j = last_i; j < i; j++) {
                new_cmd->args[k++] = is->fields[j];
            }
            new_cmd->args[k] = NULL;

            if (cmd != NULL && i != is->NF && is_token(is->fields[i])) {
                char* token = is->fields[i];
                if (strcmp(token, ">") == 0) {
                    cmd->redirect_stdout = new_cmd;

                } else if (strcmp(token, "<") == 0) {
                    cmd->redirect_stdin = new_cmd;

                } else if (strcmp(token, ">>") == 0) {
                    cmd->redirect_append_stdout = new_cmd;

                } else if (strcmp(token, "|") == 0) {
                    cmd->pipe_output = new_cmd;

                    // Update the command since we've encountered a pipe
                    cmd = new_cmd;
                }
            }

            if (cmd == NULL)
                cmd = new_cmd;

            last_i = i;
        }
    }

    return head;
}

Command* split_fields(int NF, char** fields) {
    Command* head = NULL;
    Command* last_command = NULL;
    char last_token[3];

    int i = 0;
    int ii = 0;

    for (i = 0; i < NF; i++) {
        // Find the next special token
        if (strcmp(fields[i], ">") != 0 && 
            strcmp(fields[i], "<") != 0 && 
            strcmp(fields[i], ">>") != 0 &&
            strcmp(fields[i], "|") != 0 &&
            strcmp(fields[i], "&") != 0 &&
            i + 1 != NF) continue;
        
        if (strcmp(fields[i], "&") != 0 && i + 1 == NF) i++;

        // Allocate a new command
        Command* cmd = (Command*) malloc(sizeof(Command));
        cmd->args = malloc(sizeof(char*) * (i-ii) + 1);
        cmd->argc = i-ii;

        // Extract the arguments
        int k = 0;
        int j;
        for (j = ii; j < i; j++) {
            cmd->args[k++] = strdup(fields[j]);
        }

        cmd->args[cmd->argc] = NULL;

        // Redirect output
        if (strcmp(last_token, ">") == 0) {
            last_command->redirect_stdout = cmd;

        // Redirect input
        } else if (strcmp(last_token, "<") == 0) {
            last_command->redirect_stdin = cmd;

        // Redirect output and append
        } else if (strcmp(last_token, ">>") == 0) {
            last_command->redirect_append_stdout = cmd;

        // Pipe output
        } else if (strcmp(last_token, "|") == 0) {
            last_command->pipe_output = cmd;
            last_command = cmd;

        // Run in background
        } else if (i != NF && strcmp(fields[i], "&") == 0) {
            cmd->run_in_background = 1;
        }

        if (last_command == NULL) last_command = cmd;

        if (head == NULL) head = cmd;

        if (i == NF) break;

        if (strcmp(fields[i], ">") == 0 || 
            strcmp(fields[i], "<") == 0 || 
            strcmp(fields[i], ">>") == 0 ||
            strcmp(fields[i], "|") == 0) strcpy(last_token, fields[i]);

        // Update the last position (skipping the special token)
        ii = i + 1;
    }

    return head;
}
*/

void print_command(Command* cmd) {
    int i;

    printf("\nArgs:\n");
    for (i = 0; i < cmd->argc; i++) {
        printf("  [%d]: %s\n", i, cmd->args[i]);
    }
    printf("\n");

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
        }

        if (!cmd->run_in_background)
            wait(&s1);
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

    IS is = new_inputstruct(NULL);
    while (1) {
        printf("%s", prompt);

        // Get the command from the prompt
        if (get_line(is) < 0) break;

        // Skip empty commands
        if (is->NF == 0) continue;

        print_command(get_cmd(is));
    }

    jettison_inputstruct(is);

    return 0;
}