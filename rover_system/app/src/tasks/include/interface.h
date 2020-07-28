#ifndef __INTERFACE_H 
#define __INTERFACE_H

typedef struct Interface
{
    void (*init)(void);
    void (*signal_register)(void);
    void (*message_received)(void);
    void (*update_time)(void);
    void (*end_process)(void);
}Interface_st;


#endif