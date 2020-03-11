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
 * Insert the given node into the free list by pushing it to the head
 * 
 * @param node The node to insert into the free list
 */
void free_list_insert(Flist node) {
    // If the free list is empty, this node becomes the head
    if (free_list_head == NULL) {
        free_list_head = node;
        return;
    }

    // Flist l;
    // for (l = free_list_begin(); l != NULL; l = free_list_next(l)) {
    //     if (free_list_next(l) == NULL) break;
    //     if (l->flink == NULL || (l < node && l->flink > node)) {

    //     }
    // }

    // if (free_list_next(l) == NULL) {
    //     l->flink = node;
    //     node->blink = l;
    // }

    // Replace the current head with the new node
    node->flink = free_list_head;
    free_list_head->blink = node;
    free_list_head = node;
}

/**
 * Delete the given node from the free list
 * 
 * @param node The node to delete from the free list
 */
void free_list_delete(Flist node) {
    if (node == free_list_head)
        free_list_head = node->flink;

    if (node->blink)
        node->blink->flink = node->flink;

    if (node->flink)
        node->flink->blink = node->blink;
}

void print_free_list() {
    printf("---------------- Free List ----------------\n");
    printf("%-10s    %-5s    %-10s    %-10s\n", "address", "size", "flink", "blink");

    Flist l;
    for (l = free_list_begin(); l != NULL; l = free_list_next(l)) {
        printf("0x%08x    %-5d    0x%08x    0x%08x\n", l, l->size, l->flink, l->blink);
    }

    printf("-------------------------------------------\n\n");
}

/**
 * Create a new free list node with the given size and add it to the list
 * 
 * @param size The size of the free node
 * @return Flist A pointer to the free node
 */
Flist create_new_node(size_t size) {
    size_t sbrk_size = 8192;
    if (size > 8184) sbrk_size = (size + 7 + 8) & -8;

    // Create the new node with sbrk
    Flist node = (Flist) sbrk(sbrk_size);
    node->size = sbrk_size;
    node->blink = NULL;
    node->flink = NULL;

    // Add the new node to the free list
    free_list_insert(node);

    return node;
}

/**
 * Allocate an amount of memory guaranteed to be enough to hold the given size and
 * return a pointer to the beginning of the chunk of memory
 * 
 * @param size The desired amount of memory to retrieve
 * @return void* A pointer to the beginning of the chunk of memory
 */
void *my_malloc(size_t size) {
    Flist free_node;

    // We need to allocate an extra 8 bytes for bookkeeping
    // and the size needs to be a multiple of 8
    size_t allocation = (size + 7 + 8) & -8;

    // Search for the next free node
    for (free_node = free_list_begin(); free_node != NULL; free_node = free_list_next(free_node))
        if (free_node->size >= allocation) break;

    // If a node couldn't be found, create one
    if (free_node == NULL)
        free_node = create_new_node(size);

    // Keep track of the new size of this node
    free_node->size -= allocation;

    // Remove the specified amount of memory from the bottom of the free list node
    Flist alloc_node = (void*)free_node + free_node->size;

    // Check if the node has enough for another allocation
    // If not, give the user the rest of the bytes and delete the node
    if (free_node->size <= 16) {
        allocation += free_node->size;
        free_list_delete(free_node);
    }

    // Set the size of the new chunk
    alloc_node->size = allocation;
    
    return (void*)alloc_node + 8;
}

/**
 * Free a chunk of memory used at the given pointer location
 * 
 * @param ptr The pointer to the allocated memory
 */
void my_free(void *ptr) {
    Flist node = ptr - 8;
    free_list_insert(node);
}

// int main () {
//     void *p = my_malloc(8923);
//     printf("my_malloc(%d)\n", 8923);
//     print_free_list();

//     my_free(p);
//     printf("my_free(p)\n");
//     print_free_list();

//     void *q = my_malloc(12765);
//     printf("my_malloc(%d)\n", 12765);
//     print_free_list();
// }

// int main () {
//     int *ptrs[28];
//     int *free_ptrs[14];
//     int dc[28];

//     free_ptrs[7] = my_malloc(8923);
//     ptrs[5] = my_malloc(12784); dc[5] = 12792;
//     ptrs[16] = my_malloc(12777); dc[16] = 12792;
//     ptrs[15] = my_malloc(12219); dc[15] = 12232;
//     free_ptrs[8] = my_malloc(8751);
//     free_ptrs[6] = my_malloc(8262);
//     free_ptrs[2] = my_malloc(8671);
//     free_ptrs[9] = my_malloc(8567);
//     free_ptrs[11] = my_malloc(8591);
//     ptrs[26] = my_malloc(12043); dc[26] = 12056;
//     free_ptrs[5] = my_malloc(8389);
//     free_ptrs[12] = my_malloc(8399);
//     ptrs[18] = my_malloc(12453); dc[18] = 12464;
//     free_ptrs[10] = my_malloc(8466);
//     free_ptrs[4] = my_malloc(8270);
//     free_ptrs[3] = my_malloc(8498);
//     free_ptrs[13] = my_malloc(8432);
//     ptrs[20] = my_malloc(12300); dc[20] = 12312;
//     free_ptrs[0] = my_malloc(9157);
//     ptrs[27] = my_malloc(12878); dc[27] = 12888;
//     ptrs[21] = my_malloc(12318); dc[21] = 12328;
//     ptrs[8] = my_malloc(12737); dc[8] = 12752;
//     ptrs[13] = my_malloc(12817); dc[13] = 12832;
//     free_ptrs[1] = my_malloc(8578);
//     my_free(free_ptrs[7]);
//     printf("my_free(free_ptrs[7])\n");
//     print_free_list();
//     ptrs[1] = my_malloc(12765); dc[1] = 12776;
//     ptrs[23] = my_malloc(12751); dc[23] = 12760;
//     my_free(free_ptrs[0]);
//     printf("my_free(free_ptrs[0])\n");
//     print_free_list();
//     my_free(free_ptrs[5]);
//     printf("my_free(free_ptrs[5])\n");
//     print_free_list();
//     ptrs[22] = my_malloc(12407); dc[22] = 12416;
//     ptrs[12] = my_malloc(12534); dc[12] = 12544;
//     ptrs[19] = my_malloc(12960); dc[19] = 12968;
//     ptrs[24] = my_malloc(12420); dc[24] = 12432;
//     my_free(free_ptrs[3]);
//     printf("my_free(free_ptrs[3])\n");
//     print_free_list();
//     ptrs[25] = my_malloc(12893); dc[25] = 12904;
//     ptrs[7] = my_malloc(12890); dc[7] = 12904;
//     ptrs[9] = my_malloc(12105); dc[9] = 12120;
//     ptrs[2] = my_malloc(12315); dc[2] = 12328;
//     my_free(free_ptrs[4]);
//     printf("my_free(free_ptrs[4])\n");
//     print_free_list();
//     my_free(free_ptrs[13]);
//     printf("my_free(free_ptrs[13])\n");
//     print_free_list();
//     my_free(free_ptrs[6]);
//     printf("my_free(free_ptrs[6])\n");
//     print_free_list();
//     my_free(free_ptrs[12]);
//     printf("my_free(free_ptrs[12])\n");
//     print_free_list();
//     my_free(free_ptrs[2]);
//     printf("my_free(free_ptrs[2])\n");
//     print_free_list();
//     ptrs[10] = my_malloc(12138); dc[10] = 12152;
//     my_free(free_ptrs[10]);
//     printf("my_free(free_ptrs[10])\n");
//     print_free_list();
//     my_free(free_ptrs[8]);
//     printf("my_free(free_ptrs[8])\n");
//     print_free_list();
//     ptrs[0] = my_malloc(12505); dc[0] = 12520;
//     my_free(free_ptrs[11]);
//     printf("my_free(free_ptrs[11])\n");
//     print_free_list();
//     ptrs[11] = my_malloc(12954); dc[11] = 12968;
//     my_free(free_ptrs[9]);
//     printf("my_free(free_ptrs[9])\n");
//     print_free_list();
//     ptrs[6] = my_malloc(12164); dc[6] = 12176;
//     ptrs[17] = my_malloc(12529); dc[17] = 12544;
//     my_free(free_ptrs[1]);
//     printf("my_free(free_ptrs[1])\n");
//     print_free_list();
//     ptrs[3] = my_malloc(12475); dc[3] = 12488;
//     ptrs[4] = my_malloc(12988); dc[4] = 13000;
//     ptrs[14] = my_malloc(12866); dc[14] = 12880;
// }