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
#include "dllist.h"
#include "sockettome.h"
#include <pthread.h>

typedef struct server {
    Dllist chat_rooms;
} Server;

typedef struct room {
    char* name;
    Dllist clients;
    Dllist messages;

    pthread_mutex_t *lock;
    pthread_cond_t *input_block;
} Room;

typedef struct client {
    Server* server;
    Room* chat_room;
    FILE* fin;
    FILE* fout;
    char name[100];
    int fd;
} Client;

Room* add_client(Client* client, char* room_name);

void* room_thread(void *arg);

void* client_thread(void *arg);

int main(int argc, char **argv) {
    // Usage: chat_server hydra3.eecs.utk.edu 8005 Football Bridge Politics
    if (argc < 4) {
        fprintf(stderr, "usage: %s chat-server port Chat-Room-Names ...\n", argv[0]);
        exit(1);
    }

    Server* server = (Server*) malloc(sizeof(Server));

    server->chat_rooms = new_dllist();
    Dllist tmp;

    char* hostname = argv[1];
    int port = atoi(argv[2]);

    int i;
    for (i = 0; i < (argc - 3); i++) {
        Room* room = (Room*) malloc(sizeof(Room));
        room->name = strdup(argv[3 + i]);
        room->clients = new_dllist();
        room->messages = new_dllist();
        room->lock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
        room->input_block = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
        pthread_mutex_init(room->lock, NULL);
        pthread_cond_init(room->input_block, NULL);

        dll_append(server->chat_rooms, new_jval_v((void*)room));
    }

    printf("Host: %s\n", hostname);
    printf("Port: %d\n", port);
    printf("Rooms:\n");

    pthread_t tid;
    dll_traverse(tmp, server->chat_rooms) {
        Room* room = (Room*)tmp->val.v;
        printf("  %s\n", room->name);

        pthread_create(&tid, NULL, room_thread, room);
    }

    int socket = serve_socket(port);

    // Wait for clients to join the server
    while (1) {
        int fd = accept_connection(socket);
        Client* client = (Client*) malloc(sizeof(Client));
        client->server = server;
        client->fd = fd;
        pthread_create(&tid, NULL, client_thread, client);
    }
}

Room* add_client(Client* client, char* room_name) {
    Dllist tmp;
    dll_traverse(tmp, client->server->chat_rooms) {
        Room* room = (Room*)tmp->val.v;
        if (strcmp(room->name, room_name) == 0) {
            dll_append(room->clients, new_jval_v((void*)client));
            return room;
        }
    }
    return NULL;
}

void* room_thread(void *arg) {
    Room* room = arg;

    while (1) {
        if (!dll_empty(room->messages)) {
            pthread_mutex_lock(room->lock);

            Dllist tmp1, tmp2;

            // Loop through pending messages
            dll_traverse(tmp2, room->messages) {
                char* msg = tmp2->val.s;

                // Send message to all clients
                dll_traverse(tmp1, room->clients) {
                    Client* client = (Client*)tmp1->val.v;
                    fprintf(client->fout, msg);
                }

                dll_delete_node(tmp2);
            }

            pthread_mutex_unlock(room->lock);
        } else {
            pthread_cond_wait(room->input_block, room->lock);
        }
    }
}

void* client_thread(void *arg) {
    Client* client = arg;
    client->fin = fdopen(client->fd, "r");
    client->fout = fdopen(client->fd, "w");

    Dllist tmp;

    fprintf(client->fout, "Chat Rooms:\n\n");
    dll_traverse(tmp, client->server->chat_rooms) {
        Room* room = (Room*)tmp->val.v;
        fprintf(client->fout, "%s:\n", room->name);
    }

    fprintf(client->fout, "\nEnter your chat name (no spaces):\n");
    fflush(client->fout);

    fscanf(client->fin, "%s", client->name);
    printf("New client: %s\n", client->name);

    char room_name[300];

    do {
        fprintf(client->fout, "Enter chat room:\n");
        fflush(client->fout);

        fscanf(client->fin, "%s", room_name);
        client->chat_room = add_client(client, room_name);
    } while (client->chat_room == NULL);

    printf("Client joined chat room: %s\n", room_name);

    char line[300];

    while(1) {
        while (fgets(line, 300, client->fin) != NULL) {
            pthread_mutex_lock(client->chat_room->lock);
            dll_append(client->chat_room->messages, new_jval_s(strdup(line)));
            pthread_mutex_unlock(client->chat_room->lock);
            pthread_cond_signal(client->chat_room->input_block);
            printf("loop again\n");
        }
    }

    close(client->fd);
    fclose(client->fin);
    fclose(client->fout);
    free(client);

    return NULL;
}