#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>

/*
    12.11 : 아직 확인해야할 사항들

    1. Termination 기능 재확인
    2. client가 많을 때 concurrency 문제 없는지 확인
    3. user id에 따른 각종 operation 권환 조건 확인
*/

#define MAX_CLIENTS 1024
#define MAX_SEATS 256
#define INVALID_USER -1

typedef struct {
    int user;
    int action;
    int data;
} query;

int seat_array[MAX_SEATS];      // 좌석별 예약 정보 (idx: 좌석번호, value: 유저id)
int user_logged_in[MAX_CLIENTS]; // 사용자별 로그인 여부 
int user_seat[MAX_CLIENTS];      // 사용자별 예약된 좌석 (idx: 유저id, value: 좌석번호)
int user_password[MAX_CLIENTS];  // 사용자별 비밀번호 정보
int server_socket;
pthread_mutex_t seat_mutex[MAX_SEATS];
pthread_mutex_t user_mutex;

void initialize_server() {
    for (int i = 0; i < MAX_SEATS; i++) {
        seat_array[i] = INVALID_USER;             // 좌석 예약정보 -1로 초기화
        pthread_mutex_init(&seat_mutex[i], NULL); // 좌석별 lock 초기화
    }
    for (int i = 0; i < MAX_CLIENTS; i++) {
        user_logged_in[i] = 0;           // 로그인 여부 초기화
        user_seat[i] = INVALID_USER;     // 각 유저의 예약정보 -1로 초기화
        user_password[i] = INVALID_USER; // 비밀번호 -1로 초기화
    }
    pthread_mutex_init(&user_mutex, NULL);
}

void cleanup_server() {
    printf("Clean up server.\n");
    for (int i = 0; i < MAX_SEATS; i++) {
        pthread_mutex_destroy(&seat_mutex[i]);
    }
    pthread_mutex_destroy(&user_mutex);
    printf("Now terminate server.\n");
}

void *client_handler(void *arg) {
    int client_socket = *(int *)arg;
    int curr_user = INVALID_USER; // 현재 로그인한 유저의 id
    free(arg); 
    while (1) {
        query q;
        if (recv(client_socket, &q, sizeof(q), 0) <= 0) {
            close(client_socket);
            return NULL;
        }

        int response = -1;
        switch (q.action) {
            case 1: { // Log in
                pthread_mutex_lock(&user_mutex);
                if (q.user < 0 || q.user >= MAX_CLIENTS) {
                    // 올바르지 않은 유저id
                    response = -1;
                } 
                else if (user_logged_in[q.user] == 1) {
                    // 이미 로그인되어있는 경우
                    response = -1;
                } 
                else {
                    // 첫 로그인시 passwd 저장하고 정상 로그인
                    if(user_password[q.user] == INVALID_USER) {
                        user_password[q.user] = q.data;
                        user_logged_in[q.user] = 1;
                        response = 1;
                        curr_user = q.user;
                        printf("Register user %d password %d\n", q.user, q.data);
                    }
                    else if(user_password[q.user] == q.data) {
                        user_logged_in[q.user] = 1;
                        response = 1;
                        curr_user = q.user;
                    }
                    else {
                        response = -1;
                    }
                }
                pthread_mutex_unlock(&user_mutex);
                break;
            }
            case 2: { // Reserve
                if (q.user < 0 || q.user >= MAX_CLIENTS || user_logged_in[q.user] < 0 || 
                    curr_user != q.user || q.data < 0 || q.data >= MAX_SEATS) {
                    response = -1;
                } 
                else {
                    pthread_mutex_lock(&seat_mutex[q.data]);
                    if (seat_array[q.data] == INVALID_USER) {
                        // 좌석이 비어있는 경우
                        seat_array[q.data] = q.user;
                        user_seat[q.user] = q.data;
                        response = q.data;
                    } 
                    else {
                        response = -1;
                    }
                    pthread_mutex_unlock(&seat_mutex[q.data]);
                }
                break;
            }
            case 3: { // Check reservation
                if (q.user < 0 || q.user >= MAX_CLIENTS || curr_user != q.user || user_logged_in[q.user] < 0) {
                    response = -1;
                } 
                else {
                    response = user_seat[q.user];
                }
                break;
            }
            case 4: { // Cancel reservation
                if (q.user < 0 || q.user >= MAX_CLIENTS || user_logged_in[q.user] < 0 || curr_user != q.user ||
                    user_seat[q.user] == INVALID_USER) {
                    response = -1;
                } 
                else {
                    int seat = user_seat[q.user];
                    pthread_mutex_lock(&seat_mutex[seat]);
                    seat_array[seat] = INVALID_USER;
                    user_seat[q.user] = INVALID_USER;
                    pthread_mutex_unlock(&seat_mutex[seat]);
                    response = seat;
                }
                break;
            }
            case 5: { // Log out
                pthread_mutex_lock(&user_mutex);
                if (q.user < 0 || q.user >= MAX_CLIENTS || user_logged_in[q.user] < 0 || curr_user != q.user) {
                    // 올바르지 않은 유저id
                    response = -1;
                } 
                else {
                    user_logged_in[q.user] = INVALID_USER;
                    response = 1;
                    curr_user = INVALID_USER; 
                }
                pthread_mutex_unlock(&user_mutex);
                break;
            }
            case 0: { // Terminate
                if(q.action == 0 && q.user == 0 && q.data == 0) {
                    // 모든 데이터가 0일때만 서버 종료
                    send(client_socket, seat_array, sizeof(seat_array), 0);
                    cleanup_server();
                    close(client_socket);
                    exit(0);
                }
                else {
                    response = -1;
                    break;
                }
            }
            default: {
                response = -1;
            }
        }
        send(client_socket, &response, sizeof(response), 0);
    }
}

void signal_handler(int sig) {
    cleanup_server();
    extern int server_socket; 
    close(server_socket);
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signal_handler); // ctrl+c 시그널 핸들러 설정
    initialize_server();

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(1);
    }

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }

        int *new_sock = malloc(sizeof(int));
        *new_sock = client_socket;

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, client_handler, new_sock);
        pthread_detach(client_thread); // thread 자원 자동 해제
    }

    cleanup_server();
    close(server_socket);
    return 0;
}
