#include <includes.h>
#include <signal/signal.h>
#include <log/log.h>
#include <protocol.h>
#include <rover_manager.h>
#include <context.h>

#define ROVER_MANAGER   "ROVER_MANAGER"

static Context_st manager_context;

static void init(void);
static void message_received(int s);
static void update_time(int s);
static void end_manager(int s);

static void emitSignal(const char *proc_name);

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
  protocol_t proto;
  
  
  
  init();  

  while(1)
  {
    if(queue_recv(manager_context.mem->queueid, &manager_context.queue, sizeof(manager_context.queue.bData), 0) < 0){
      logger(LOGGER_INFO, ROVER_MANAGER, "Queue receive error.");
      continue;
    } 

    //convert to generic type to analise which id is.    
    memset(&proto, 0, sizeof(proto));

    protocol_umount(&proto, manager_context.queue.bData, sizeof(proto));
    loggerArgs(LOGGER_INFO, ROVER_MANAGER, "Received: id: %04d size: %04d payload: %s checksum: %04d", proto.id,
                                                                                           proto.size,
                                                                                           proto.payload,
                                                                                           proto.checksum);

    if(manager_route(proto.id, proto.payload) != 0){
      logger(LOGGER_INFO, ROVER_MANAGER, "Error type no exist.");
    }

    memset(manager_context.queue.bData, 0, sizeof(manager_context.queue.bData));
  }
}

int manager_route(int id, const char *command)
{   
  char proc[PROC_NAME_MAX] = {0};

  switch(id){
    case MOTOR_ID:
        strncpy(proc, ROVER_PROCESS_MOTOR, strlen(ROVER_PROCESS_MOTOR));
      break;

    case SERVO_ID:
      strncpy(proc, ROVER_PROCESS_SERVO, strlen(ROVER_PROCESS_SERVO));
      break;

    case ULTRASOUND_ID:
      strncpy(proc, ROVER_PROCESS_ULTRASOUND, strlen(ROVER_PROCESS_ULTRASOUND));
      break;

    case LCD16_ID:
      strncpy(proc, ROVER_PROCESS_LCD16, strlen(ROVER_PROCESS_LCD16));
      break;
      
    
    default: 
      return -1;
  }

  if (semaphore_lock(&manager_context.sema) == 0)
  {
    memset(manager_context.mem->msg.command, 0, sizeof(manager_context.mem->msg.command));
    memcpy(manager_context.mem->msg.command, command, strlen(command));
    semaphore_unlock(&manager_context.sema);
  }

  emitSignal(proc);
  return EXIT_SUCCESS;
}

static void emitSignal(const char *proc_name)
{
  for(int i = 0; i < PROCESS_AMOUNT; i++)
  {
    process_st *p = &manager_context.mem->processes[i];
    if(!strcmp(p->name, proc_name) && p->pid != -1)
    {
      notify_process(p->pid, SIGMESSAGERECEIVED);
      break;
    }
  }
}


static void init(void)
{
  memset(&manager_context, 0, sizeof(Context_st));

  manager_context.mem = mem_get();
  if(!manager_context.mem)
  {
    logger(LOGGER_INFO, ROVER_MANAGER, "Memory not initialized");
    exit(EXIT_FAILURE);
  }

  manager_context.sema.id = -1;
  manager_context.sema.sema_count = 1;
  manager_context.sema.state = LOCKED;
  manager_context.sema.master = SLAVE;

  semaphore_init(&manager_context.sema, SEMA_ID);
  
  signal_register(update_time, SIGUPDATETIME);
  signal_register(end_manager, SIGTERM);

  manager_context.queue.queueType = 1;

  logger(LOGGER_INFO, ROVER_MANAGER, "MANAGER initialized");
}

static void update_time(int s)
{
  manager_context.states.update_time = 1;
}

void end_manager(int s)
{
  logger(LOGGER_INFO, ROVER_MANAGER, "MANAGER unlaunched");
  exit(s);
}