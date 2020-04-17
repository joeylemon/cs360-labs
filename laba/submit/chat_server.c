/**
 * Lab A -- Chat server
 * COSC360
 * Joey Lemon
 * Apr 26, 2020
 * 
 * This program implements a chat server using pthreads that allows clients to chat with each other using nc
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "dllist.h"
#include "jrb.h"
#include "sockettome.h"

typedef struct server {
    JRB chat_rooms;
} Server;

typedef struct room {
    char* name;
    Dllist clients;
    Dllist messages;

    pthread_t tid;
    pthread_mutex_t *lock;
    pthread_cond_t *new_message;
} Room;

typedef struct client {
    Room* chat_room;
    pthread_t tid;

    int fd;
    FILE* fin;
    FILE* fout;
    char name[100];
} Client;

Server* server;

/**
 * The function that runs room threads
 * 
 * @param arg A pointer to a room
 */
void* room_thread(void *arg);

/**
 * The function that runs client threads
 * 
 * @param arg A pointer to a client
 */
void* client_thread(void *arg);

/**
 * Add a client to the given room
 * 
 * @param client The client struct
 * @param room_name The name of the room
 * @return Room* The pointer to the room the client was added to
 */
Room* client_join_room(Client* client, char* room_name);

/**
 * Send a message from the client to all clients in the room
 * 
 * @param client The client struct
 * @param line The input line to send as a message
 */
void client_send_message(Client* client, char* line);

/**
 * Free all memory for a given client
 * 
 * @param client The client struct
 */
void free_client(Client* client);

/**
 * Free all memory for a given room
 * 
 * @param room The room struct
 */
void free_room(Room* room);

int main(int argc, char **argv) {
    // Command line checking
    if (argc < 3) {
        fprintf(stderr, "usage: %s port Chat-Room-Names ...\n", argv[0]);
        exit(1);
    }

    // Set up a shared info struct
    server = (Server*) malloc(sizeof(Server));
    server->chat_rooms = make_jrb();

    // Allocate each room given in the arguments list
    int i;
    for (i = 0; i < (argc - 2); i++) {
        Room* room = (Room*) malloc(sizeof(Room));
        room->name = strdup(argv[2 + i]);
        room->clients = new_dllist();

        // Initialize all mutexes and conditions
        room->lock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
        room->new_message = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
        pthread_mutex_init(room->lock, NULL);
        pthread_cond_init(room->new_message, NULL);

        // Add the room to the shared info struct
        jrb_insert_str(server->chat_rooms, room->name, new_jval_v((void*)room));

        // Create a thread for each room
        if (pthread_create(&room->tid, NULL, room_thread, room) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    int port = atoi(argv[1]);
    char hostname[100];
    gethostname(hostname, 100);

    // Print server information
    printf("Listening on port :%d\n", port);
    printf("Connect with:\n");
    printf("  \e[36mnc %s %d\e[0m\n", hostname, port);
    printf("Grade with:\n");
    printf("  \e[36m~jplank/cs360/labs/laba/gradeall %s %d\e[0m\n", hostname, port);

    int socket = serve_socket(port);

    // Wait for clients to join the server
    while (1) {
        int fd = accept_connection(socket);
        Client* client = (Client*) malloc(sizeof(Client));
        client->fd = fd;

        // Create a thread for each client
        if (pthread_create(&client->tid, NULL, client_thread, client) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    JRB tmp;
    jrb_traverse(tmp, server->chat_rooms) {
        Room* room = (Room*)tmp->val.v;
        pthread_detach(room->tid);
        free_room(room);
    }

    jrb_free_tree(server->chat_rooms);
    free(server);

    return 0;
}

void* room_thread(void *arg) {
    Room* room = arg;

    while (1) {
        room->messages = new_dllist();

        pthread_mutex_lock(room->lock);

        // Wait for a new message from a client
        while (dll_empty(room->messages))
            pthread_cond_wait(room->new_message, room->lock);

        Dllist tmp1, tmp2;

        // Loop through pending messages
        dll_traverse(tmp2, room->messages) {
            if(tmp2 == NULL) break;

            char* msg = tmp2->val.s;

            // Send message to all clients
            dll_traverse(tmp1, room->clients) {
                Client* client = (Client*)tmp1->val.v;
                fprintf(client->fout, msg);
                fflush(client->fout);
            }
        }

        dll_traverse(tmp2, room->messages)
            free(tmp2->val.s);
        free_dllist(room->messages);

        pthread_mutex_unlock(room->lock);
    }

    // Thread has finished, free memory
    free_room(room);

    return NULL;
}

void* client_thread(void *arg) {
    Client* client = arg;

    // Open the socket for reading
    client->fin = fdopen(client->fd, "r");
    if (client->fin == NULL) {
        perror("fdopen");
        exit(1);
    }

    // Open the socket for writing
    client->fout = fdopen(client->fd, "w");
    if (client->fout == NULL) {
        perror("fdopen");
        exit(1);
    }

    Dllist dtmp;
    JRB jtmp;
    char buf[350];

    // Print the room names and their lists of clients
    fprintf(client->fout, "Chat Rooms:\n\n");

    jrb_traverse(jtmp, server->chat_rooms) {
        Room* room = (Room*)jtmp->val.v;

        sprintf(buf, "%s:", room->name);

        pthread_mutex_lock(room->lock);

        // Loop through all clients in the room and build a list
        dll_traverse(dtmp, room->clients) {
            Client* c = (Client*)dtmp->val.v;
            strcat(buf, " ");
            strcat(buf, c->name);
        }

        pthread_mutex_unlock(room->lock);

        strcat(buf, "\n");
        fprintf(client->fout, buf);
    }

    // Prompt the user for their name
    fprintf(client->fout, "\nEnter your chat name (no spaces):\n");
    fflush(client->fout);
    if (fscanf(client->fin, "%s", client->name) != 1) {
        free_client(client);
        pthread_exit(NULL);
    }

    // Prompt the user for the room they want to join
    do {
        fprintf(client->fout, "Enter chat room:\n");
        fflush(client->fout);

        if (fscanf(client->fin, "%s", buf) != 1) {
            free_client(client);
            pthread_exit(NULL);
        }

        client->chat_room = client_join_room(client, buf);

        // Continously loop until the user chooses a proper room
        if (client->chat_room == NULL)
            fprintf(client->fout, "No chat room %s.\n", buf);
    } while (client->chat_room == NULL);

    char line[300];

    // Send a join message to all clients
    sprintf(buf, "%s has joined\n", client->name);
    client_send_message(client, buf);

    // Continously check for input from the client
    while (fgets(line, 300, client->fin) != NULL) {
        // Ignore empty lines
        if (strcmp(line, "\n") == 0) continue;

        // Send the line to all clients
        sprintf(buf, "%s: %s", client->name, line);
        client_send_message(client, buf);
    }

    // Since the loop ended, the user has quit
    pthread_mutex_lock(client->chat_room->lock);

    // Remove the user from the list of clients
    dll_traverse(dtmp, client->chat_room->clients) {
        Client* c = (Client*)dtmp->val.v;
        if (strcmp(client->name, c->name) == 0) {
            dll_delete_node(dtmp);
            break;
        }
    }

    // Send a leave message to all clients
    sprintf(buf, "%s has left\n", client->name);
    dll_append(client->chat_room->messages, new_jval_s(strdup(buf)));

    pthread_cond_signal(client->chat_room->new_message);
    pthread_mutex_unlock(client->chat_room->lock);


    free_client(client);

    return NULL;
}

Room* client_join_room(Client* client, char* room_name) {
    JRB tmp;

    jrb_traverse(tmp, server->chat_rooms) {
        Room* room = (Room*)tmp->val.v;

        // Loop through the list of rooms until we find the given name
        if (strcmp(room->name, room_name) == 0) {
            pthread_mutex_lock(room->lock);

            // Add the client to the room's list of clients
            dll_append(room->clients, new_jval_v((void*)client));

            pthread_mutex_unlock(room->lock);
            return room;
        }
    }

    return NULL;
}

void client_send_message(Client* client, char* msg) {
    pthread_mutex_lock(client->chat_room->lock);

    // Add the message to the list
    dll_append(client->chat_room->messages, new_jval_s(strdup(msg)));

    // Signal to the room that there is a new message
    pthread_cond_signal(client->chat_room->new_message);

    pthread_mutex_unlock(client->chat_room->lock);
}

void free_client(Client* client) {
    // Close all open files
    close(client->fd);
    fclose(client->fin);
    fclose(client->fout);

    // Free the struct
    free(client);
}

void free_room(Room* room) {
    Dllist tmp;

    // Free messages (used strdup)
    dll_traverse(tmp, room->messages)
        free(tmp->val.s);

    // Free clients
    dll_traverse(tmp, room->clients) {
        Client* c = (Client*)tmp->val.v;
        free_client(c);
    }
        
    // Free name and list
    free(room->name);
    free(room->lock);
    free(room->new_message);
    free_dllist(room->messages);
    free_dllist(room->clients);
    free(room);
}