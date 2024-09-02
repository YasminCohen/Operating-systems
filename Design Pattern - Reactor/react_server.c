#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <poll.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 9034
#define MAX_FDS 8192

typedef int (*handler_t)(int, void *);

struct node
{
	int data;
	handler_t handler;
	struct node *next;
};

struct reactor_struct
{
	pthread_t thread;
	struct node *head;
	int hot;
};

void *createReactor();
void startReactor(void *reactor);
void stopReactor(void *reactor);
void addFd(void *reactor, int fd, handler_t handler);
void WaitFor(void *reactor);

int client_handler(int fd, void *reactor) {
    char buffer[1024] = {0};

    if (reactor == NULL) {
        return -1;
    }

    int ret = recv(fd, buffer, sizeof(buffer), 0);

    if (ret == -1) {
        perror("recv()");
        return -1;
    }

    if (ret == 0) {
        printf("Client %d disconnected\n", fd);
        return 1;
    }

    if (ret == sizeof(buffer)) {
        buffer[ret - 1] = '\0';
    }

    else {
        buffer[ret] = '\0';
    }

    printf("client %d: %s\n", fd, buffer);

    return 0;
}

int server_handler(int fd, void *reactor) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_len);

    if (client_fd == -1) {
        perror("accept()");
        return -1;
    }

    printf("Client %s:%d connected. Ref ID: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_fd);

    addFd(reactor, client_fd, client_handler);

    return 0;
}

int main(void) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;

    if (server_fd == -1) {
        perror("socket()");
        exit(1);
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt()");
        exit(1);
    }

    int ret = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (ret == -1) {
        perror("bind()");
        exit(1);
    }

    ret = listen(server_fd, MAX_FDS);

    if (ret == -1) {
        perror("listen()");
        exit(1);
    }

    void *reactor = createReactor();

    if (reactor == NULL) {
        exit(1);
    }

    addFd(reactor, server_fd, server_handler);

    startReactor(reactor);

    WaitFor(reactor);

    if (close(server_fd) == -1) {
        perror("close()");
        exit(1);
    }

    struct reactor_struct *reactor_ptr = (struct reactor_struct *)reactor;

    while (reactor_ptr->head != NULL) {
        struct node *cur = reactor_ptr->head;
        reactor_ptr->head = cur->next;
        close(cur->data);
        free(cur);
    }

    free(reactor);

    return 0;
}