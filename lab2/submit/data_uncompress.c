/* Lab 1 -- Fields/dlist/rbtree primer
   COSC360
   This program reads data from a binary file with a
   nasty format and prints it out cleanly
   Joey Lemon
   2020-02-09 */

#include <stdio.h>
#include <stdlib.h>

/**
    Returns the next number of elements to read
    Errors:
        when the n value couldn't be read
**/
int get_next_n() {
    int c = getchar();

    if (c == EOF) {
        fprintf(stderr, "Input error: no size\n");
        exit(1);
    }

    return c;
}

/**
    Returns the next number of chars to read for a string
    Errors:
        when the size value couldn't be read
**/
int get_next_size() {
    int size = getchar();

    if (size == EOF) {
        fprintf(stderr, "Input error: no string size\n");
        exit(1);
    }

    return size;
}

/**
    Reads the next n integers into the buffer
    Errors:
        when less than n integers were read
**/
void read_next_ints(int *intbuf, int n) {
    int nread = fread(intbuf, sizeof(int), n + 1, stdin);

    if (nread != (n + 1)) {
        fprintf(stderr, "Input error: not enough ints\n");
        exit(1);
    }
}

/**
    Reads the next n doubles into the buffer
    Errors:
        when less than n doubles were read
**/
void read_next_doubles(double *doublebuf, int n) {
    int nread = fread(doublebuf, sizeof(double), n + 1, stdin);

    if (nread != (n + 1)) {
        fprintf(stderr, "Input error: not enough doubles\n");
        exit(1);
    }
}

/**
    Reads the next s_size chars into the buffer
    Errors:
        when less than s_size chars were read
**/
void read_next_string(char *charbuf, int s_size) {
    int nread = fread(charbuf, sizeof(char), s_size + 1, stdin);

    if (nread != (s_size + 1)) {
        fprintf(stderr, "Input error: not enough chars\n");
        exit(1);
    }

    /* Add null terminator */
    charbuf[s_size + 1] = '\0';
}

int main() {
    int c, last_c, i, n, s_size;
    last_c = 0;

    char charbuf[256]; /* 256 size, extra space for '\0' */
    int intbuf[255];
    double doublebuf[255];

    while (1) {
        /* Get the next character that represents what type of data to read
           s=string, n=newline, i=integers, d=doubles */
        c = getchar();

        /* If we've reached the end of the file, we've finished the process */
        if (c == EOF) exit(0);

        /* Read a string */
        if (c == 's') {
            n = get_next_n();

            /* Get the next n strings */
            for (i = 0; i <= n; i++) {
                s_size = get_next_size();

                read_next_string(charbuf, s_size);

                if (i != 0 || (last_c != 'n' && last_c != 0)) printf(" ");
                printf("%s", charbuf);
            }

        /* Read the next integers */
        } else if (c == 'i') {
            n = get_next_n();

            read_next_ints(intbuf, n);

            /* Print the n ints read */
            for (i = 0; i <= n; i++) {
                if (i != 0 || (last_c != 'n' && last_c != 0)) printf(" ");
                printf("%d", intbuf[i]);
            }

        /* Read the next doubles */
        } else if (c == 'd') {
            n = get_next_n();

            read_next_doubles(doublebuf, n);

            /* Print the n doubles read */
            for (i = 0; i <= n; i++) {
                if (i != 0 || (last_c != 'n' && last_c != 0)) printf(" ");
                printf("%.10lg", doublebuf[i]);
            }

        /* Print a newline */
        } else if (c == 'n') {
            printf("\n");

        /* If we've reached this point, there's a bad type of data */
        } else {
            fprintf(stderr, "Input error: bad type\n");
            exit(1);
        }

        /* Keep track of the last data type we read */
        last_c = c;
    }
}