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
    pid_t pid;
    char* redirect_stdout;
    char* redirect_append_stdout;
    char* redirect_stdin;
    struct cmd *pipe_input;
    struct cmd *pipe_output;

    int pipe_in_fd;
    int pipe_out_fd;
} Command;

/**
 * Clear all memory of the command to default
 * 
 * @param cmd The command to clear
 */
void command_clear(Command* cmd) {
    int i;
    for (i = 0; i < 100; i++) {
        cmd->args[i] = NULL;
    }

    cmd->argc = 0;
    cmd->run_in_background = 0;
    cmd->completed = 0;
    cmd->pid = 0;
    cmd->redirect_stdout = NULL;
    cmd->redirect_append_stdout = NULL;
    cmd->redirect_stdin = NULL;
    cmd->pipe_input = NULL;
    cmd->pipe_output = NULL;
    cmd->pipe_in_fd = 0;
    cmd->pipe_out_fd = 0;
}

/**
 * Check if the command is still running
 * 
 * @param cmd The command to check
 * @return int 1 if the command is still running, 0 if not
 */
int is_waiting(Command* cmd) {
    if (!cmd->completed) return 1;

    Command* c = cmd->pipe_output;
    while (c != NULL) {
        if (!c->completed) return 1;
        c = c->pipe_output;
    }

    return 0;
}

/**
 * Set the given pid as completed
 * 
 * @param head The head command
 * @param pid The pid to set as completed
 */
void set_completed(Command* head, pid_t pid) {
    Command* c = head;
    while (c != NULL) {
        if (c->pid == pid) {
            c->completed = 1;
            return;
        }

        c = c->pipe_output;
    }
}

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

    // Since we've scanned all the redirects, check if there's a background flag as well
    if (i < is->NF && strcmp(is->fields[i], "&") == 0)
        cmd->run_in_background = 1;

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

    command_clear(cmd);

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
 * Free the command and all of its pipes
 * 
 * @param cmd The command
 */
void free_cmd(Command* cmd) {
    int i;
    int count = 0;

    Command* c = cmd;
    while (c != NULL) {
        Command* nc = c->pipe_output;

        if (c->redirect_stdout)         free(c->redirect_stdout);
        if (c->redirect_append_stdout)  free(c->redirect_append_stdout);
        if (c->redirect_stdin)          free(c->redirect_stdin);
        for (i = 0; i < c->argc; i++) {
            free(c->args[i]);
        }

        if (count) free(c);

        c = nc;
        count++;
    }
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

        if (new_cmd->argc > 0) {
            if (cmd != NULL) {
                cmd->pipe_output = new_cmd;
                new_cmd->pipe_input = cmd;
            }

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

void execute(Command* cmd) {
    if (cmd->pipe_input != NULL) {
        if (dup2(cmd->pipe_in_fd, 0) != 0) {   
            perror("jsh: dup2(cmd->pipe_in_fd)");
            exit(1);
        }
    }

    if (cmd->pipe_output != NULL) {
        if (dup2(cmd->pipe_out_fd, 1) != 1) {
            perror("jsh: dup2(cmd->pipe_out_fd)");
            exit(1);
        }
    }

    if (cmd->pipe_in_fd)    close(cmd->pipe_in_fd);
    if (cmd->pipe_out_fd)   close(cmd->pipe_out_fd);

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

    execvp(cmd->args[0], cmd->args);
    perror(cmd->args[0]);
    exit(1);
}

void run(Command* cmd) {
    int i = 0;
    Command* c = cmd;

    while (c != NULL) {
        if (c->pipe_output != NULL) {
            int pipefd[2];
            if (pipe(pipefd) < 0) {
                perror("jsh: pipe");
                exit(1);
            }
            
            c->pipe_out_fd = pipefd[1];
            c->pipe_output->pipe_in_fd = pipefd[0];
        }

        c->pid = fork();

        // Child process
        if (c->pid == 0) {

            execute(c);

        // Parent process
        } else {
            if (c->pipe_in_fd)  close(c->pipe_in_fd);
            if (c->pipe_out_fd) close(c->pipe_out_fd);

            if (c->run_in_background) c->completed = 1;
        }

        c = c->pipe_output;
        i++;
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
        // Print the prompt
        printf("%s", prompt);

        // Get the input from the prompt
        if (get_line(is) < 0) break;

        // Skip empty commands
        if (is->NF == 0) continue;

        cmd = get_cmd(is);
        
        run(cmd);

        while (is_waiting(cmd)) {
            pid_t pid = wait(NULL);
            set_completed(cmd, pid);
        }

        free_cmd(cmd);
    }

    jettison_inputstruct(is);

    return 0;
}