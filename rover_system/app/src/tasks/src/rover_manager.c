#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <manager.h>
#include <queue/queue.h>
#include <sharedmemory/sharedmemory.h>
#include <rover_types.h>
#include <signal.h>
#include <signal/signal.h>
#include <log/log.h>
#include <app.h>
#include <protocol.h>
#include <semaphore/semaphore.h>
#include <rover_manager.h>

#define ROVER_MANAGER   "ROVER_MANAGER"

static int queue_id = -1;
static  sema_t sema = {
        .id = -1,
        .sema_count = 1,
        .state = LOCKED,
        .master = SLAVE
    };

static void signal_handler(int sig){
  if(sig == SIGTERM){
    logger(LOGGER_INFO, ROVER_MANAGER, "Manager unlaunched");
    exit(SIGTERM);
  }
}

static void emitSignal(const char *proc_name, MEM *mem);

#ifdef PROCESS
int main(int argc, char const *argv[])
{
    (void)rover_manager(NULL);
    return 0;
}
#endif

void *rover_manager(void *args)
{
  (void)args;
   MEM *mem = NULL;
  protocol_t proto;
  queue_st queue;
  
  signal_register(signal_handler, SIGTERM);

  queue_id = queue_init(QUEUE_MANAGER_ID);

  if(queue_id < 0){
    logger(LOGGER_INFO, ROVER_MANAGER, "Queue init error.");
    exit(EXIT_FAILURE);
  }

  semaphore_init(&sema, SEMA_ID);

  mem = mem_get();
  if(mem == NULL)
  {
    logger(LOGGER_INFO, ROVER_MANAGER, "Memory not initialized");
    return 1;
  }
  
  queue.queueType = 1;

  while(1)
  {
    if(queue_recv(queue_id, &queue, sizeof(queue.bData), 0) < 0){
      logger(LOGGER_INFO, ROVER_MANAGER, "Queue receive error.");
      continue;
    } 

    //convert to generic type to analise which id is.    
    memset(&proto, 0, sizeof(proto));

    protocol_umount(&proto, queue.bData, sizeof(proto));
    loggerArgs(LOGGER_INFO, ROVER_MANAGER, "Received: id: %04d size: %04d payload: %s checksum: %04d", proto.id,
                                                                                           proto.size,
                                                                                           proto.payload,
                                                                                           proto.checksum);

    if(manager(proto.id, proto.payload, mem) != 0){
      logger(LOGGER_INFO, ROVER_MANAGER, "Error type no exist.");
    }

    memset(queue.bData, 0, sizeof(queue.bData));
  }
}

int manager(int id, const char *command, MEM *mem)
{   
  char proc[PROC_NAME_MAX] = {0};
  generic_st *dev = NULL;

  switch(id){
    case MOTOR_ID:
      dev = &mem->motor;
      strncpy(proc, ROVER_PROCESS_MOTOR, strlen(ROVER_PROCESS_MOTOR));
      break;

    case SERVO_ID:
      dev = &mem->servo;
      strncpy(proc, ROVER_PROCESS_SERVO, strlen(ROVER_PROCESS_SERVO));
      break;

    case ULTRASOUND_ID:
      dev = &mem->ultrasound;
      strncpy(proc, ROVER_PROCESS_ULTRASOUND, strlen(ROVER_PROCESS_ULTRASOUND));
      break;

    case LCD16_ID:
      dev = &mem->lcd16;
      strncpy(proc, ROVER_PROCESS_LCD16, strlen(ROVER_PROCESS_LCD16));
      break;
      
    
    default: 
      return -1;
  }

  if (semaphore_lock(&sema) == 0)
  {
    memset(dev->command, 0, sizeof(dev->command));
    memcpy(dev->command, command, strlen(command));
    semaphore_unlock(&sema);
  }

  emitSignal(proc, mem);
  return EXIT_SUCCESS;
}

static void emitSignal(const char *proc_name, MEM *mem)
{
  for(int i = 0; i < PROCESS_AMOUNT; i++)
  {
    process_t *p = &mem->procs[i];
    if(!strcmp(p->name, proc_name) && p->pid != -1)
    {
      notify_process(p->pid, SIGUSR1);
      break;
    }
  }
}
