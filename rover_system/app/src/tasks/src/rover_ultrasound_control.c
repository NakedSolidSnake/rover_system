#include <includes.h>
#include <log/log.h>
#include <signal/signal.h>
#include <ultrasound_action.h>
#include <hc_sr04.h>
#include <rover_ultrasound_control.h>
#include <context.h>

#define ROVER_ULTRASOUND   "ROVER_ULTRASOUND"

static Context_st ultrasound_context;

static void init(void);
static void message_received(int s);
static void update_time(int s);
static void end_ultrasound(int s);


#ifdef PROCESS
int main(int argc, char const *argv[])
{
    (void)rover_ultrasound_control(NULL);
    return 0;
}
#endif

void *rover_ultrasound_control(void *args)
{
  (void)args;
 
  init();
  
  while(1)
  {
    if(ultrasound_context.states.message_received){
      memcpy(&ultrasound_context.msg, &ultrasound_context.mem->msg, sizeof(message_st));
      logger(LOGGER_INFO, ROVER_ULTRASOUND, ultrasound_context.msg.command);
      //call command here      
      if (semaphore_lock(&ultrasound_context.sema) == 0)
      {
        ultrasound_action_select(ultrasound_context.msg.command, strlen(ultrasound_context.msg.command));
        snprintf(ultrasound_context.queue.bData, sizeof(ultrasound_context.queue.bData) ,"$:%04d:%04d:%s:FFFF:#",
                                                                                        ULTRASOUND_ID,
                                                                                        (int)strlen(ultrasound_context.msg.command),
                                                                                        ultrasound_context.msg.command);
        queue_send(ultrasound_context.mem->queue_server_id, &ultrasound_context.queue, (int)strlen(ultrasound_context.msg.command) + 1);
        semaphore_unlock(&ultrasound_context.sema);
      }
      ultrasound_context.states.message_received = 0;
    } 

    else if(ultrasound_context.states.update_time){

      ultrasound_context.states.update_time = 0;
    }

    else{
      pause();
    }
  }

  return NULL;
}

static void init(void)
{
  HC_SR04_init();

  memset(&ultrasound_context, 0, sizeof(Context_st));

  ultrasound_context.mem = mem_get();
  if(!ultrasound_context.mem)
  {
    logger(LOGGER_INFO, ROVER_ULTRASOUND, "Memory not initialized");
    exit(EXIT_FAILURE);
  }

  ultrasound_context.sema.id = -1;
  ultrasound_context.sema.sema_count = 1;
  ultrasound_context.sema.state = LOCKED;
  ultrasound_context.sema.master = SLAVE;

  semaphore_init(&ultrasound_context.sema, SEMA_ID);
  
  signal_register(message_received, SIGMESSAGERECEIVED);
  signal_register(update_time, SIGUPDATETIME);
  signal_register(end_ultrasound, SIGTERM);

  logger(LOGGER_INFO, ROVER_ULTRASOUND, "ULTRASOUND initialized");
}

void message_received(int s)
{  
  ultrasound_context.states.message_received = 1;
}

static void update_time(int s)
{
  ultrasound_context.states.update_time = 1;
}

void end_ultrasound(int s)
{
  logger(LOGGER_INFO, ROVER_ULTRASOUND, "ULTRASOUND unlaunched");
  exit(s);
}
