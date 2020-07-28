#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <rover_types.h>
#include <log/log.h>
#include <signal/signal.h>
#include <signal.h>
#include <ultrasound_action.h>
#include <hc_sr04.h>
#include <app.h>
#include <semaphore/semaphore.h>
#include <queue/queue.h>
#include <rover_ultrasound_control.h>

#define ROVER_ULTRASOUND   "ROVER_ULTRASOUND"

static int _update = 0;

static sema_t sema = {
        .id = -1,
        .sema_count = 1,
        .state = LOCKED,
        .master = SLAVE
    };

static void update(int s);
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
  queue_st queue;
  message_st ultrasound;
  MEM *mem = NULL;

  HC_SR04_init();

  semaphore_init(&sema, SEMA_ID);

  signal_register(update, SIGUSR1);
  signal_register(end_ultrasound, SIGTERM);

  mem = mem_get();
  if(!mem)
  {
    logger(LOGGER_INFO, ROVER_ULTRASOUND, "Memory not initialized");
    return 1;
  }

  logger(LOGGER_INFO, ROVER_ULTRASOUND, "Ultrasound initialized");
  
  while(1)
  {
    if(_update == 1){
      memcpy(&ultrasound, &mem->msg, sizeof(ultrasound));
      logger(LOGGER_INFO, ROVER_ULTRASOUND, ultrasound.command);
      //call command here      
      if (semaphore_lock(&sema) == 0)
      {
        ultrasound_action_select(ultrasound.command, strlen(ultrasound.command));
        snprintf(queue.bData, sizeof(queue.bData) ,"$:%04d:%04d:%s:FFFF:#", ULTRASOUND_ID, (int)strlen(ultrasound.command), ultrasound.command);
        queue_send(mem->queue_server_id, &queue, (int)strlen(ultrasound.command) + 1);
        semaphore_unlock(&sema);
      }
      _update = 0;
    } 
    else{
      pause();
    }
  }
}

static void update(int s)
{
  _update = 1;
}

static void end_ultrasound(int s)
{
  logger(LOGGER_INFO, ROVER_ULTRASOUND, "Ultrasound unlaunched");
  exit(s);
}