#ifndef __ROVER_TYPES_H
#define __ROVER_TYPES_H 

typedef struct message{
  int id;
  int status;
  char command[64];
}message_st;


#endif
