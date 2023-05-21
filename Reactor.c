#pragma once
#include "Reactor.h"

void* createReactor(){

    P_reactor reactor = malloc(sizeof(Reactor));
// return to this line :(
    reactor->handlers = malloc(sizeof(P_handler_t));
   // reactor->handlers[0] = malloc();
    reactor->size = 0;
    reactor->is_runing = 0;
    reactor->pol_fd = malloc(sizeof(struct pollfd));

    return reactor;
}

void stopReactor(void * this){

    if (this->is_runing == 1)
    {
        this->is_runing = 0;
        WaitFor(this);
    }


}

void WaitFor(void * this){

    if (this->is_runing == 0)
    {
        pthread_join(this->chat_thread, NULL);
    }

}

void startReactor( P_reactor this){

    if (this-> is_runing ==0)
    {
        this-> is_runing = 1;
        pthread_create(&this->chat_thread, NULL, func, this);
    }

    
}
void* func(void* react){

    P_reactor reactor = (P_reactor) react;
    while (reactor->is_runing == 1)
    {
        int poli = poll(reactor->pol_fd, reactor->nfd, -1); 

        if (poli < 0)
        {
            printf("poll problem\n");
            return NULL;
        }
        else
        {
            for (int i = 0; i < reactor->nfd; i++)
            {
                // reactor->handlers[i] = malloc(sizeof(handler_t));
                // if(! reactor->handlers[i] ){
                //     printf("malloc start\n");
                //     return NULL;
                // }
                reactor->handlers[i]->handler(reactor->handlers[i]->arguments);
                reactor->handlers[i]->my_P_reactor = reactor;
                reactor->handlers[i]->handler_fd = reactor->pol_fd->fd;  
            }
            
        }
        
        
    }
    

}








