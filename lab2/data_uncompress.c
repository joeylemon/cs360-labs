/**
 * Lab 2 -- Reading nasty data
 * COSC360
 * Joey Lemon
 * Feb 9, 2020
 * 
 * This program reads data from a binary file with a
 * "compressed" format and prints out its data
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Gets the next number of elements to read
 * This will exit the program if the end of stdin is reached
 * 
 * @return a positive integer representing how many elements to read
 */
int get_next_n();

/**
 * Get the size (# of chars) of the upcoming string
 * This will exit the program if the end of stdin is reached
 * 
 * @return a positive integer representing how many chars to read
 */
int get_next_size();

/**
 * Read the next (n + 1) integers from stdin into the buffer
 * This will exit the program if <(n + 1) integers were able to be read
 * 
 * @param intbuf A buffer to read integers into
 * @param n The number of integers to read from stdin
 */
void read_next_ints(int *intbuf, int n);

/**
 * Read the next (n + 1) doubles from stdin into the buffer
 * This will exit the program if <(n + 1) doubles were able to be read
 * 
 * @param intbuf A buffer to read doubles into
 * @param n The number of doubles to read from stdin
 */
void read_next_doubles(double *doublebuf, int n);

/**
 * Read the next (s_size + 1) chars from stdin into the string
 * This will exit the program if <(s_size + 1) chars were able to be read
 * 
 * @param charbuf A char buffer to read characters into
 * @param n The number of chars to read from stdin
 */
void read_next_string(char *charbuf, int s_size);



int main() {
    int c, last_c, i, n, s_size;
    last_c = 0;

    char charbuf[256]; /* 256 size, extra space for '\0' */
    int intbuf[255];
    double doublebuf[255];

    while (1) {
        /* Get the next character that represents what type of data to read
           's'=string, 'n'=newline, 'i'=integers, 'd'=doubles */
        c = getchar();

        /* If we've reached the end of the file, we've finished the process */
        if (c == EOF) 
            exit(0);

        /* If we're reading a data type, the next char represents how many data
           elements need to be read */
        if (c == 's' || c == 'i' || c == 'd')
            n = get_next_n();

        /* Read a string */
        if (c == 's') {
            /* Get the next n strings */
            for (i = 0; i <= n; i++) {
                s_size = get_next_size();

                read_next_string(charbuf, s_size);

                if (i != 0 || (last_c != 'n' && last_c != 0))
                    printf(" ");
                    
                printf("%s", charbuf);
            }

        /* Read the next integers */
        } else if (c == 'i') {
            read_next_ints(intbuf, n);

            /* Print the n ints read */
            for (i = 0; i <= n; i++) {
                if (i != 0 || (last_c != 'n' && last_c != 0))
                    printf(" ");

                printf("%d", intbuf[i]);
            }

        /* Read the next doubles */
        } else if (c == 'd') {
            read_next_doubles(doublebuf, n);

            /* Print the n doubles read */
            for (i = 0; i <= n; i++) {
                if (i != 0 || (last_c != 'n' && last_c != 0))
                    printf(" ");

                printf("%.10lg", doublebuf[i]);
            }

        /* Print a newline */
        } else if (c == 'n') {
            printf("\n");

        /* If we've reached this point, there's a type of data
           that isn't in {'s', 'i', 'd', 'n'}, which is invalid */
        } else {
            fprintf(stderr, "Input error: bad type\n");
            exit(1);
        }

        /* Keep track of the last data type we read */
        last_c = c;
    }
}



int get_next_n() {
    int c = getchar();

    if (c == EOF) {
        fprintf(stderr, "Input error: no size\n");
        exit(1);
    }

    return c;
}

int get_next_size() {
    int size = getchar();

    if (size == EOF) {
        fprintf(stderr, "Input error: no string size\n");
        exit(1);
    }

    return size;
}

void read_next_ints(int *intbuf, int n) {
    int nread = fread(intbuf, sizeof(int), n + 1, stdin);

    if (nread != (n + 1)) {
        fprintf(stderr, "Input error: not enough ints\n");
        exit(1);
    }
}

void read_next_doubles(double *doublebuf, int n) {
    int nread = fread(doublebuf, sizeof(double), n + 1, stdin);

    if (nread != (n + 1)) {
        fprintf(stderr, "Input error: not enough doubles\n");
        exit(1);
    }
}

void read_next_string(char *charbuf, int s_size) {
    int nread = fread(charbuf, sizeof(char), s_size + 1, stdin);

    if (nread != (s_size + 1)) {
        fprintf(stderr, "Input error: not enough chars\n");
        exit(1);
    }

    /* Add null terminator */
    charbuf[s_size + 1] = '\0';
}
