#ifndef __APP_H
#define __APP_H

#include <sharedmemory/sharedmemory.h>
#include <semaphore/semaphore.h>
#include <queue/queue.h>
#include <rover_status.h>
#include <rover_types.h>
#include <sys/types.h>
#include <time.h>

#define PROC_NAME_MAX     80

// Queue id's
#define QUEUE_MANAGER_ID  100
#define QUEUE_SERVER_ID   101

// Semaphore id's
#define SEMA_MESSAGE_ID           1234 
#define SEMA_UPDATE_ID            1235 

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
#define PROCESS_AMOUNT 9

#define MONITOR_MAX_TRY   3

#define MONITOR_CICLE_SECONDS   6 

#define PROCESS_CICLE_SECONDS   3

#define MQTT_CICLE_SECONDS      3

typedef struct process
{
  int pid;
  char name[PROC_NAME_MAX];
  time_t update;
  time_t old; 
  unsigned char miss_count;

}process_st;

typedef struct MEM
{
  int init;
  process_st processes[PROCESS_AMOUNT];
  shm_t shm;
  int queueid;
  int queue_server_id;
  sema_t sema_message;
  sema_t sema_update;
  message_st msg;
  Status_st status; 
  int process_amount;
  unsigned char monitor_enable : 1; 
  unsigned char RFU : 7;
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

int get_pid(pid_t pid);


#endif
