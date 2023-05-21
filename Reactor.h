#pragma once
#include <pthread.h>
#include <sys/poll.h>
#include <stdlib.h>




typedef struct handler_t
{   
    void* (*handler)(void*);

    void* arguments;

    int handler_fd;

    P_reactor my_P_reactor;

} handler_t, *P_handler_t;

typedef struct Reactor
{

    struct pollfd* pol_fd;

    int nfd;

    int is_runing;

    pthread_t chat_thread;

    P_handler_t* handlers;

    int size;

} Reactor, *P_reactor ;

void* createReactor();

void stopReactor(void * this);

void startReactor(void * this);

void addFd (void * this,int fd, handler_t handler);

void WaitFor(void * this);












