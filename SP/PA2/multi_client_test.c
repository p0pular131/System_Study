#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

typedef struct _query {
    int user;
    int action;
    int seat;
} query;

#define MAX_CLIENTS 1024  
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

void *client_thread(void *arg) {
    int user_id = *(int *)arg;
    free(arg);

    int client_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_socket);
        pthread_exit(NULL);
    }

    query q;
    q.user = user_id;
    q.action = 1; 
    q.seat = 1234; 
    send(client_socket, &q, sizeof(q), 0);

    int response;
    recv(client_socket, &response, sizeof(response), 0);
    printf("User %d Login Response: %d\n", user_id, response);

    q.action = 2; 
    q.seat = user_id % 256; 
    send(client_socket, &q, sizeof(q), 0);
    recv(client_socket, &response, sizeof(response), 0);
    printf("User %d Reserve Seat %d Response: %d\n", user_id, q.seat, response);

    q.action = 5; 
    q.seat = 0;   
    send(client_socket, &q, sizeof(q), 0);
    recv(client_socket, &response, sizeof(response), 0);
    printf("User %d Logout Response: %d\n", user_id, response);

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[MAX_CLIENTS];

    for (int i = 0; i < MAX_CLIENTS; i++) {
        int *user_id = malloc(sizeof(int));
        *user_id = i;
        if (pthread_create(&threads[i], NULL, client_thread, user_id) != 0) {
            perror("Thread creation failed");
            exit(1);
        }
    }

    for (int i = MAX_CLIENTS-1; i < MAX_CLIENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All client threads completed.\n");
    return 0;
}
