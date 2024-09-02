#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <poll.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

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

void *reactor_run(void *arg) {
    struct reactor_struct *reactor_ptr = (struct reactor_struct *)arg;
    while (reactor_ptr->hot == 1) {
        struct pollfd fds[MAX_FDS];
        int i = 0;
        struct node *cur = reactor_ptr->head;
        while (cur != NULL) {
            fds[i].fd = cur->data;
            fds[i].events = POLLIN;
            i++;
            cur = cur->next;
        }
        int ret = poll(fds, i, -1);
        if (ret == -1) {
            perror("poll()");
            exit(1);
        }
        for (int j = 0; j < i; j++) {
            if (fds[j].revents & POLLIN) {
                cur = reactor_ptr->head;
                int ret = 0;
                while (cur != NULL) {
                    if (cur->data == fds[j].fd) {
                        ret = cur->handler(fds[j].fd, reactor_ptr);
                        break;
                    }
                    cur = cur->next;
                }

                if (ret == -1) {
                    perror("handler()");
                    exit(1);
                }

                else if (ret == 1) {
                    struct node *prev = NULL;
                    cur = reactor_ptr->head;
                    while (cur != NULL) {
                        if (cur->data == fds[j].fd) {
                            if (prev == NULL) {
                                reactor_ptr->head = cur->next;
                            } else {
                                prev->next = cur->next;
                            }
                            free(cur);
                            break;
                        }
                        prev = cur;
                        cur = cur->next;
                    }
                }
            }
        }
    }

    printf("Reactor exited\n");
    return NULL;
}

void *createReactor() {
    struct reactor_struct *reactor = malloc(sizeof(struct reactor_struct));

    if (reactor == NULL) {
        perror("malloc()");
        return NULL;
    }

    reactor->head = NULL;
    reactor->hot = 0;

    printf("Reactor created\n");

    return reactor;
}

void startReactor(void *reactor) {
    struct reactor_struct *reactor_ptr = (struct reactor_struct *)reactor;

    if (reactor_ptr->hot == 1) {
        return;
    }
    reactor_ptr->hot = 1;
    int ret = pthread_create(&reactor_ptr->thread, NULL, reactor_run, reactor_ptr);

    if (ret != 0) {
        errno = ret;
        perror("pthread_create()");
        exit(1);
    }

    printf("Reactor started\n");
}
void stopReactor(void *reactor) {
    struct reactor_struct *reactor_ptr = (struct reactor_struct *)reactor;

    if (reactor_ptr->hot == 0) {
        return;
    }

    reactor_ptr->hot = 0;

    int ret = pthread_cancel(reactor_ptr->thread);

    if (ret != 0) {
        errno = ret;
        perror("pthread_cancel()");
        exit(1);
    }

    ret = pthread_join(reactor_ptr->thread, NULL);

    if (ret != 0) {
        errno = ret;
        perror("pthread_join()");
        exit(1);
    }

    printf("Reactor stopped\n");
}
void addFd(void *reactor, int fd, handler_t handler) {
    struct reactor_struct *reactor_ptr = (struct reactor_struct *)reactor;
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL) {
        perror("malloc()");
        exit(1);
    }
    new_node->data = fd;
    new_node->handler = handler;
    new_node->next = NULL;
    if (reactor_ptr->head == NULL) {
        reactor_ptr->head = new_node;
    } else {
        struct node *cur = reactor_ptr->head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = new_node;
    }

    printf("Fd %d added\n", fd);
}
void WaitFor(void *reactor) {
    struct reactor_struct *reactor_ptr = (struct reactor_struct *)reactor;

    printf("Joining reactor thread\n");

    int ret = pthread_join(reactor_ptr->thread, NULL);
    if (ret != 0) {
        errno = ret;
        perror("pthread_join()");
        exit(1);
    }
}