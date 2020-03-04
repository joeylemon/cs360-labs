/**
 * Lab 7 -- Jmalloc
 * COSC360
 * Joey Lemon
 * Mar 22, 2020
 * 
 * This program implements a custom version of malloc() and free() that can
 * be used to allocate dynamic memory from the heap at runtime
 */

#include "mymalloc.h"

typedef struct flist {
   int size;
   struct flist *flink;
   struct flist *blink;
} *Flist;

Flist free_list_head = NULL;

/**
 * Round a number up to the nearest multiple
 * 
 * @param n The number to round
 * @param multiple The multiple
 * @return int The rounded number
 */
int roundUp(int n, int multiple) {
    if (multiple == 0) return n;

    int remainder = abs(n) % multiple;
    if (remainder == 0) return n;

    if (n < 0)
        return -(abs(n) - remainder);
    else
        return n + multiple - remainder;
}

/**
 * Get the head of the free list
 * 
 * @return void* The pointer to the first Flist element
 */
void *free_list_begin() {
    return free_list_head;
}

/**
 * Follow the flink of the given pointer to an Flist element
 * 
 * @param node The element to get the successive element of
 * @return void* The pointer to the next Flist element
 */
void *free_list_next(void *node) {
    Flist flist = node;
    return flist->flink;
}

/**
 * Allocate an amount of memory guaranteed to be equal to the given size and
 * return a pointer to the beginning of the chunk of memory
 * 
 * @param size The desired amount of memory to retrieve
 * @return void* A pointer to the beginning of the chunk of memory
 */
void *my_malloc(size_t size) {
    void *p;
    Flist free_node;

    // We need to allocate an extra 8 bytes for bookkeeping
    // and the size needs to be a multiple of 8
    size_t allocation = roundUp(size + 8, 8);

    // If free_list_head is NULL, then the free list is empty and we need to
    // use sbrk to get memory to use
    if (free_list_head == NULL) {
        size_t sbrk_size = 8192;
        if (size > 8184) sbrk_size = allocation;

        free_node = (Flist) sbrk(sbrk_size);
        free_node->size = sbrk_size;
        free_node->blink = NULL;
        free_node->flink = NULL;

        free_list_head = free_node;

    // Find the next free node that has enough memory to hold size
    } else {
        Flist last_node = free_list_head;
        free_node = free_list_head;
        while (free_node->size < allocation) {
            free_node = free_list_next(free_node);

            // If we've reached the end of the free list, get a new big chunk of memory
            if (free_node == NULL) {
                size_t sbrk_size = 8192;
                if (size > 8184) sbrk_size = allocation;

                free_node = (Flist) sbrk(sbrk_size);
                free_node->size = sbrk_size;
                free_node->blink = NULL;
                free_node->flink = NULL;

                last_node->flink = free_node;
                
                break;
            }
            
            last_node = free_node;
        }
    }

    // Remove the specified amount of memory from the bottom of the free list node
    void *new_chunk = (void*)free_node + free_node->size - allocation;

    // Set the size of the new chunk
    *((int*)new_chunk) = allocation;

    // Keep track of the new size of this node
    free_node->size -= allocation;

    if (free_node->size == 0) {
        size_t sbrk_size = 8192;

        Flist new_node = (Flist) sbrk(sbrk_size);
        new_node->size = sbrk_size;
        new_node->blink = free_node->blink;
        new_node->flink = free_node->flink;

        if (free_list_head == free_node) {
            free_list_head = new_node;
        } else {
            if(free_node->blink) free_node->blink->flink = new_node;
            if(free_node->flink) free_node->flink->blink = new_node;
        }
    }
    
    return new_chunk + 8;
}

void my_free(void *ptr) {
    Flist node = ptr - 8;

    // If the free list is empty, this node becomes the head
    if (free_list_head == NULL) {
        free_list_head = node;
        return;
    }

    // Add the node to the end of the free list
    Flist free_node = free_list_head;
    while (1) {
        if (free_node->flink == NULL) {
            free_node->flink = node;
            node->blink = free_node;
            break;
        }

        free_node = free_node->flink;
    }
}

void print_free_list() {
    if (free_list_head == NULL) return;

    printf("---------------- Free List ----------------\n");
    printf("%-10s    %-5s    %-10s    %-10s\n", "address", "size", "flink", "blink");

    Flist free_node = free_list_head;
    while (free_node != NULL) {
        printf("0x%08x    %-5d    0x%08x    0x%08x\n", free_node, free_node->size, free_node->flink, free_node->blink);
        free_node = free_node->flink;
    }

    printf("-------------------------------------------\n\n");
}

// int main() {
//     void* p = my_malloc(16);
//     void* q = my_malloc(1024);
//     printf("&p: 0x%08x\n", p);
//     printf("&q: 0x%08x\n", q);
//     printf("&q - &p: 0x%08x (%d)\n", q - p, q - p);
//     print_free_list();

//     my_free(p);
//     printf("my_free(p)\n");
//     print_free_list();

//     my_free(q);
//     printf("my_free(q)\n");
//     print_free_list();

//     void* r = my_malloc(400);
//     printf("r = my_malloc(400)     0x%08x\n", r);
//     print_free_list();

//     void* s = my_malloc(400);
//     printf("s = my_malloc(400)     0x%08x\n", s);
//     print_free_list();

//     void* t = my_malloc(400);
//     printf("t = my_malloc(400)     0x%08x\n", t);
//     print_free_list();

//     void* u = my_malloc(400);
//     printf("u = my_malloc(400)     0x%08x\n", u);
//     print_free_list();
// 	return 0;
// }

// int main() {
//     void* p = my_malloc(23);
//     printf("my_malloc(23)\n");
//     print_free_list();

//     void* q = my_malloc(8145);
//     printf("my_malloc(8145)\n");
//     print_free_list();

//     my_free(p);
//     printf("my_free(p)\n");
//     print_free_list();

// 	return 0;
// }