#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char name[100];
    int x, y;
    int cur_PP, max_PP;
    struct node *prev;

    int adj_size;
    struct node **adj;

    char visited;
    int healing;
} Node;

typedef struct global {
    // Command-line arguments
    int initial_range, jump_range, num_jumps, initial_power;
    float power_reduction;

    // Path-finding information
    int best_healing;
    int best_path_length;
    Node **best_path;
    int *healing;
} Global;

// Calculate the distance between two nodes using basic distance formula
float distance(Node *n1, Node *n2) {
    return sqrt(((n2->x - n1->x) * (n2->x - n1->x)) + ((n2->y - n1->y) * (n2->y - n1->y)));
}

// Perform a depth-first-search
// n                source node
// from             recursive reference to last source node
// global           pointer to global struct
// hop              number of hops made thus far
// total_healing    how much has been healed thus far
void DFS(Node *n, Node *from, Global *global, int hop, int total_healing) {
    if (n->visited || hop > global->num_jumps) return;

    n->prev = from;
    n->visited = 1;

    // Calculate how much the node can be healed
    // Using compound interest equation
    int healing = rint((global->initial_power) * pow(1 - global->power_reduction, hop - 1));
    if (healing + n->cur_PP > n->max_PP)
        healing = n->max_PP - n->cur_PP;

    n->healing = healing;
    total_healing += healing;

    // If this path has the largest healing potential, trace the path
    if (total_healing > global->best_healing) {
        global->best_healing = total_healing;
        global->best_path_length = 0;

        Node *cur = n;
        for (int i = 0; i < global->num_jumps && cur != NULL; i++) {
            global->best_path_length++;
            global->best_path[i] = cur;
            global->healing[i] = cur->healing;

            cur = cur->prev;
        }
    }

    // Loop through this node's neighbors
    for (int i = 0; i < n->adj_size; i++) {
        // Recursively perform DFS on this node
        DFS(n->adj[i], n, global, hop + 1, total_healing);
    }

    n->visited = 0;
    n->prev = NULL;
}

int main(int argc, char **argv) {
    if (argc != 6) {
        fprintf(stderr, "usage: %s initial_range jump_range num_jumps initial_power power_reduction < input_file\n", argv[0]);
        return 1;
    }

    // Read command line arguments and convert to numbers
    Global global;
    global.initial_range = atoi(argv[1]);
    global.jump_range = atoi(argv[2]);
    global.num_jumps = atoi(argv[3]);
    global.initial_power = atoi(argv[4]);
    global.power_reduction = atof(argv[5]);

    // Scan input into nodes
    int node_count = 0;
    Node *node = (Node *)malloc(sizeof(Node));
    node->prev = NULL;
    while (scanf("%d %d %d %d %s", &node->x, &node->y, &node->cur_PP, &node->max_PP, node->name) == 5) {
        Node *prev = node;
        node = (Node *)malloc(sizeof(Node));
        node->prev = prev;

        node_count++;
    }

    // Create the array of nodes by traversing prev pointers
    Node *nodes[node_count];
    for (int i = 0; i < node_count; i++) {
        node = node->prev;
        nodes[i] = node;
    }

    // Keep a reference to Urgosa since he starts the chain heal
    // Could maybe just use nodes[0], but we search for name to be safe
    Node *urgosa;

    // Loop through all nodes and create their adjacency lists
    for (int i = 0; i < node_count; i++) {
        Node *n = nodes[i];
        n->adj_size = 0;

        // Calculate size of adjacency list
        for (int j = 0; j < node_count; j++) {
            // A node cannot be adjacent to itself
            if (i == j) continue;

            // A node must be reachable to be adjacent
            if (distance(n, nodes[j]) > global.jump_range) continue;

            n->adj_size++;
        }

        // Allocate array of n=adj_size adjacent nodes
        n->adj = (Node **)malloc(sizeof(Node *) * n->adj_size);

        // Counter to track index of adjacency list
        int x = 0;

        // Populate adjacency list
        for (int j = 0; j < node_count; j++) {
            // A node cannot be adjacent to itself
            if (i == j) continue;

            // A node must be reachable to be adjacent
            if (distance(n, nodes[j]) > global.jump_range) continue;

            n->adj[x++] = nodes[j];
        }

        // Keep a reference to Urgosa
        if (strcmp(n->name, "Urgosa_the_Healing_Shaman") == 0) urgosa = n;
    }

    // Allocate path tracking
    global.best_healing = 0;
    global.best_path_length = 0;
    global.best_path = (Node **)malloc(sizeof(Node *) * global.num_jumps);
    global.healing = (int *)malloc(sizeof(int *) * global.num_jumps);

    // Find nodes within initial_range of Urgosa
    for (int i = 0; i < node_count; i++) {
        // Set all nodes to unvisited
        for (int j = 0; j < node_count; j++) {
            nodes[j]->visited = 0;
            nodes[j]->healing = 0;
            nodes[j]->prev = NULL;
        }

        // Find nodes in initial range to start chain heal on
        if (distance(urgosa, nodes[i]) <= global.initial_range) {
            DFS(nodes[i], NULL, &global, 1, 0);
        }
    }

    for (int i = global.best_path_length - 1; i >= 0; i--) {
        printf("%s %d\n", global.best_path[i]->name, global.healing[i]);
    }

    printf("Total_Healing %d\n", global.best_healing);

    // Free all memory that has been allocated
    for (int i = 0; i < node_count; i++) {
        free(nodes[i]->adj);
        free(nodes[i]);
    }

    free(global.best_path);
    free(global.healing);
}
