#ifndef __ROVER_TYPES_H
#define __ROVER_TYPES_H 

typedef struct motor{
  int id;
  int status;
  char command[64];
}motor_st;

typedef struct servo{
  int id;
  int status;
  char command[64];
}servo_st;

#define MOTOR_OFFSET      0
#define SERVO_OFFSET      (MOTOR_OFFSET + sizeof(motor_st))

#endif
