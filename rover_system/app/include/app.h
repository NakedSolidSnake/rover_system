#ifndef __APP_H
#define __APP_H

#include <sharedmemory/sharedmemory.h>
#include <semaphore/semaphore.h>
#include <rover_status.h>
#include <rover_types.h>
#include <time.h>

#define PROC_NAME_MAX     80

// Queue id's
#define QUEUE_MANAGER_ID  100
#define QUEUE_SERVER_ID   101

// Semaphore id's
#define SEMA_ID           1234 

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
#define ROVER_PROCESS_LCD16         "/home/cssouza/rover/bin/rover_lcd16_control"
#define ROVER_PROCESS_MANAGER       "/home/cssouza/rover/bin/rover_manager"
#define ROVER_PROCESS_MQTT          "/home/cssouza/rover/bin/rover_mqtt"
#define ROVER_PROCESS_WEBSERVER     "/home/cssouza/rover/bin/rover_webserver"
#define ROVER_PROCESS_MONITOR       "/home/cssouza/rover/bin/rover_monitor"

//Process amount
#define PROCESS_AMOUNT 8

typedef struct process
{
  int pid;
  char name[PROC_NAME_MAX];
  time_t update;
  time_t old; 
  int miss_count;

}process_st;

typedef struct MEM
{
  int init;
  process_st processes[PROCESS_AMOUNT];
  shm_t shm;
  int queueid;
  int queue_server_id;
  sema_t sema;
  message_st msg;
  Status_st status; 
  int process_amount; 
}MEM;

typedef enum TYPE{
  MOTOR_ID = 0,
  SERVO_ID,
  ULTRASOUND_ID,
  LCD16_ID
}TYPE_e;


int mem_init(void);
int mem_denit(void);
MEM *mem_get(void);
int memoryWrite(MEM * mem, void *data, int offset);

int ipc_init(void);
int ipc_denit(void);

#endif
