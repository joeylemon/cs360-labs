#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#define PMT pthread_mutex_t
#define PCT pthread_cond_t

typedef struct {
    PMT *m;
    PCT **c;
    int n;      /* Number of threads */
    int count;  /* How many threads are ready */
} Shared;

typedef struct {
    int id;     /* Thread id, from 0 to (n-1) */
    Shared *s;  
} Info;

PMT *new_mutex() {
    PMT *m;
    m = (PMT *) malloc(sizeof(PMT));
    pthread_mutex_init(m, NULL);
    return m;
}

PCT *new_cond() {
    PCT *c;
    c = (PCT *) malloc(sizeof(PCT));
    pthread_cond_init(c, NULL);
    return c;
}

int perform_calculation(int id) {
    int j = 0;

    int i = 0;
    for (i = 0; i < 100000000; i++) {
        j += id;
    }

    printf("perform_calculation for thread %d\n", id);
    return j;
}

int update_state(int id) {
    int j = 0;

    int i = 0;
    for (i = 0; i < 100000000; i++) {
        j += id;
    }

    printf("update_state for thread %d\n", id);
    return j;
}

// Joey Lemon
// Question 3
void* thread(void* arg) {
    int i;
    Info* ti = (Info*) arg;
    Shared* s = ti->s;

    while (1) {
        perform_calculation(ti->id);

        pthread_mutex_lock(s->m);
        s->count++;
        
        // Wait for all threads to perform calculation
        if (s->count != s->n) {
            pthread_cond_wait(s->c[ti->id], s->m);
        } else {
            s->count = 0;
            for (i = 0; i < s->n; i++)
                pthread_cond_signal(s->c[i]);
        }

        update_state(ti->id);
        s->count++;

        // Wait for all threads to update state
        if (s->count != s->n) {
            pthread_cond_wait(s->c[ti->id], s->m);
        } else {
            s->count = 0;
            for (i = 0; i < s->n; i++)
                pthread_cond_signal(s->c[i]);
        }
        
        pthread_mutex_unlock(s->m);
    }

    return NULL;
}

// Joey Lemon
// Question 3
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s n\n", argv[0]);
        exit(1);
    }

    pthread_t tid;
    int i;

    Shared* s = (Shared*) malloc(sizeof(Shared));
    s->m = new_mutex();
    s->count = 0;
    s->n = atoi(argv[1]);
    s->c = (PCT**) malloc(sizeof(PCT) * s->n);

    for (i = 0; i < s->n; i++)
        s->c[i] = new_cond();
    
    for (i = 0; i < s->n; i++) {
        Info* ti = (Info*) malloc(sizeof(Info));
        ti->id = i;
        ti->s = s;

        pthread_create(&tid, NULL, thread, (void*) ti);
    }

    pthread_exit(NULL);
}