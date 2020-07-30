#ifndef __CONTEXT_H
#define __CONTEXT_H

#include <app.h>
#include <queue/queue.h>
#include <rover_types.h>
#include <semaphore/semaphore.h>
#include <sys/time.h> 

typedef struct State
{
    unsigned char update_time : 1;
    unsigned char message_received : 1;
    unsigned char RFU : 6;
}States_st;

typedef struct Context
{    
    MEM *mem;
    queue_st queue;
    message_st msg;
    sema_t sema_message;
    sema_t sema_update;
    States_st states;
    struct timespec current; 
}Context_st;

#endif