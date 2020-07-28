#ifndef __INTERFACE_H 
#define __INTERFACE_H

#include <app.h>
#include <queue/queue.h>
#include <rover_types.h>
#include <semaphore/semaphore.h>

typedef struct Context
{    
    MEM *mem;
    queue_st queue;
    message_st msg;
    sema_t sema;

}Context_st;

typedef struct Interface
{
    void (*init)(void);
    void (*signal_register)(void);
    void (*message_received)(void);
    void (*update_time)(void);
    void (*end_process)(void);
}Interface_st;


#endif