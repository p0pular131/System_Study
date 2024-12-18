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

#define MAX_CLIENTS 1024  // 생성할 클라이언트 수
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

    // 로그인 요청
    query q;
    q.user = user_id;
    q.action = 1; // Login action
    q.seat = 1234; // Password as an example
    send(client_socket, &q, sizeof(q), 0);

    int response;
    recv(client_socket, &response, sizeof(response), 0);
    printf("User %d Login Response: %d\n", user_id, response);

    // 좌석 예약 요청
    q.action = 2; // Reserve action
    q.seat = user_id % 256; // 좌석 번호는 user_id 기반으로 설정
    send(client_socket, &q, sizeof(q), 0);
    recv(client_socket, &response, sizeof(response), 0);
    printf("User %d Reserve Seat %d Response: %d\n", user_id, q.seat, response);

    // 예약 확인 요청
    q.action = 3; // Check reservation action
    q.seat = 0;   // Data 필드는 필요 없음
    send(client_socket, &q, sizeof(q), 0);
    recv(client_socket, &response, sizeof(response), 0);
    printf("User %d Check Reservation Response: %d\n", user_id, response);

    // 로그아웃 요청
    q.action = 5; // Logout action
    q.seat = 0;   // Data 필드는 필요 없음
    send(client_socket, &q, sizeof(q), 0);
    recv(client_socket, &response, sizeof(response), 0);
    printf("User %d Logout Response: %d\n", user_id, response);

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[MAX_CLIENTS];

    // 스레드 생성 및 실행
    for (int i = 0; i < MAX_CLIENTS; i++) {
        int *user_id = malloc(sizeof(int));
        *user_id = i;
        if (pthread_create(&threads[i], NULL, client_thread, user_id) != 0) {
            perror("Thread creation failed");
            exit(1);
        }
    }

    // 모든 스레드가 종료될 때까지 대기
    for (int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All client threads completed.\n");
    return 0;
}
