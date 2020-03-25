/* This program performs the following command:
   "cat < f1 > f2" */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/* File table entries:
   0: stdin
   1: stdout
   2: stderr */

/* When you call dup2(fd, 0), 
   then whenever your program reads from standard input, it will read from fd. */

/* When you call dup2(fd, 1), 
   then whenever your program reads from standard output, it will read from fd. */

int main(int argc, char **argv, char **envp)
{
  int fd1, fd2;
  int dummy;
  char *newargv[2];

  /* In the child, first open f1 and then dup it to file descriptor zero. */

  if (fork() == 0) {
    fd1 = open("f1", O_RDONLY);
    if (fd1 < 0) {
      perror("catf1f2: f1");
      exit(1);
    }
  
    if (dup2(fd1, 0) != 0) {
      perror("catf1f2: dup2(f1, 0)");
      exit(1);
    }
    close(fd1);
  
    /* In the child, next open f2 and then dup it to file descriptor one. */

    fd2 = open("f2", O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd2 < 0) {
      perror("catf1f2: f2");
      exit(2);
    }
  
    if (dup2(fd2, 1) != 1) {
      perror("catf1f2: dup2(f2, 1)");
      exit(1);
    }
    close(fd2);

    /* Set up the argv array, and call execve(). */
 
    newargv[0] = "cat";
    newargv[1] = (char *) 0;

    execve("/bin/cat", newargv, envp);
    perror("execve(bin/cat, newargv, envp)");
    exit(1);  

  /* The parent merely waits for the child to exit. */

  } else {
    wait(&dummy);
  }
  return 0;
}