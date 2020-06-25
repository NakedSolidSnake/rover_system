#ifndef __APP_H
#define __APP_H

#include <sharedmemory/sharedmemory.h>

#define PROC_NAME_MAX     80

// Queue id's
#define QUEUE_MANAGER_ID  100

// Semaphore id's
 

// Shared Memory id's

#define SHM_MOTOR_ID    1
#define SHM_SERVO_ID    2
#define SHM_ULTRASOM_ID 3

#define SHM_PROCESSES_KEY   4

//process names
#define ROVER_PROCESS_SERVER        "/home/cssouza/rover/bin/rover_server"
#define ROVER_PROCESS_MOTOR         "/home/cssouza/rover/bin/rover_motor_control"
#define ROVER_PROCESS_SERVO         "/home/cssouza/rover/bin/rover_servo_control"
#define ROVER_PROCESS_ULTRASOUND    "/home/cssouza/rover/bin/rover_ultrasound_control"
#define ROVER_PROCESS_MANAGER       "/home/cssouza/rover/bin/rover_manager"
#define ROVER_PROCESS_MQTT          "/home/cssouza/rover/bin/rover_mqtt"
#define ROVER_PROCESS_WEBSERVER     "/home/cssouza/rover/bin/rover_webserver"

//Process amount
#define PROCESS_AMOUNT 7

typedef struct process
{
  int pid;
  char name[PROC_NAME_MAX];
}process_t;

typedef struct generic{
  int id;
  int status;
  char command[64];
}generic_st;

typedef struct MEM
{
  int init;
  process_t procs[PROCESS_AMOUNT];
  shm_t shm;
  int queueid;
  generic_st motor;
  generic_st servo;
  generic_st ultrasound;
}MEM;


int mem_init(MEM *mem);
MEM *mem_get(void);
int mem_set(MEM *mem);

#endif
