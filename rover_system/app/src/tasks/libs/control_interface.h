#ifndef __CONTROL_INTERFACE_H
#define __CONTROL_INTERFACE_H

#include <context.h>

typedef struct Control 
{
    int (*hardwareInit)(void);
    int (*actionSelect)(const char *action, int actionLength);
}IControl;


int controlExec(Context_st *context, IControl *controlContext);

#endif