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
 * Performs an insertion-sort to keep nodes in order
 * 
 * @param node The node to insert into the free list
 */
void free_list_insert(Flist node) {
    // If the free list is empty, this node becomes the head
    if (free_list_head == NULL) {
        free_list_head = node;
        return;
    }

    // Find the next node where the new node should be inserted
    Flist l;
    for (l = free_list_begin(); l->flink != NULL && node > l->flink; l = l->flink);

    // If we reached the end of the list, simply append the new node
    if (l->flink == NULL) {
        l->flink = node;
        node->blink = l;
        return;
    }

    // Otherwise, insert the new node in the place of l
    l->flink->blink = node;
    node->flink = l->flink;

    l->flink = node;
    node->blink = l;
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
 * Combine all adjacent entries in the free list into a single node
 */
void coalesce_free_list() {
    Flist l = free_list_begin();
    while (l != NULL) {
        Flist next = l->flink;

        // If the next node is adjacent, combine them
        if ((void*)l + l->size == next) {
            l->size += next->size;
            free_list_delete(next);
        } else {
            l = l->flink;
        }
    }
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