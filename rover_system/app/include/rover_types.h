#ifndef __ROVER_TYPES_H
#define __ROVER_TYPES_H 

#include <signal.h>

typedef struct message{
  int id;
  int status;
  char command[64];
}message_st;

#define SIGUPDATETIME       SIGUSR1
#define SIGMESSAGERECEIVED  SIGUSR2

#endif
