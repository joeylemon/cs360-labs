/* This program performs the following command:
   "head -5 headsort.c | sort" */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/* Exec function calls:

execl(char *path, char *arg0, char *arg1, ..., char *argn, NULL): This uses your current envp, and lets you specify the argv as parameters, rather than building an array of pointers. Path must specify the path name exactly.

execv(char *path, char **argv): This is just like execve, but uses your current envp.

execle(char *path, char *arg0, char *arg1, ..., char *argn, NULL, char **envp): This is just like execl, but you must specify envp.

execve(char *path, char **argv, char **envp).

execlp(char *path, char *arg0, char *arg1, ..., char *argn, NULL): This is just like execl, except that if path is a relative filename, then the directories in your PATH variable will be searched to find path.

execvp(char *path, char **argv): This is just like execv, except that the PATH variable will be searched to find path. */

/* File table entries:
   0: stdin
   1: stdout
   2: stderr */

/* Pipe: Anything that is written on fd[1] may be read by fd[0] */

int main()
{
  int dummy2, dummy1, i;
  int pipefd[2];

  /* First you have to create the pipe in the parent, so that the children can share it. */

  i = pipe(pipefd);
  if (i < 0) {
    perror("headsort: pipe");
    exit(1);
  }

  /* The first child dups the write end of the pipe to standard output. */

  if (fork() == 0) {
    if (dup2(pipefd[1], 1) != 1) {
      perror("headsort: dup2(pipefd[1])");
      exit(1);
    }

    close(pipefd[1]);
    close(pipefd[0]);

    fprintf(stderr, "Execcing head\n");
    execlp("head", "head", "-5", "headsort.c", NULL);
    perror("headsort: exec head failed");
    exit(1);

  /* The second child dups the read end of the pipe to standard input. */

  } else if (fork() == 0) {
    if (dup2(pipefd[0], 0) != 0) {   
      perror("headsort: dup2(pipefd[0])");
      exit(1);
    }

    close(pipefd[1]);
    close(pipefd[0]);

    printf("Execcing sort\n");
    execlp("sort", "sort", NULL);
    perror("headsort: exec sort failed");
    exit(1);

  /* The parent closes all ends of the pipe, and then waits for the children. */

  } else {
    close(pipefd[1]);
    close(pipefd[0]);

    
    wait(&dummy1);
    wait(&dummy2);
    if (WEXITSTATUS(dummy1) || WEXITSTATUS(dummy2)) {
      fprintf(stderr, "Abnormal exit.\n");
    }
  }
  return 0;
}