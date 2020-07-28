#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <rover_types.h>
#include <log/log.h>
#include <signal/signal.h>
#include <signal.h>
#include <lcd16_action.h>
#include <lcd16.h>
#include <app.h>
#include <semaphore/semaphore.h>
#include <queue/queue.h>
#include <rover_lcd16_control.h>

#define ROVER_LCD16   "ROVER_LCD16"

static int _update = 0;

static sema_t sema = {
        .id = -1,
        .sema_count = 1,
        .state = LOCKED,
        .master = SLAVE
    };

void update(int s);
void end_lcd16(int s);
void segfault(int s);

#ifdef PROCESS
int main(int argc, char const *argv[])
{
    (void)rover_lcd16_control(NULL);
    return 0;
}
#endif


void *rover_lcd16_control(void *args)
{
  (void)args;
  message_st lcd16;
  MEM *mem = NULL;
  queue_st queue;

  semaphore_init(&sema, SEMA_ID);

  LCD16_init();

  signal_register(update, SIGUSR1);
  signal_register(end_lcd16, SIGTERM);
  signal_register(segfault, SIGUSR2);

  mem = mem_get();
  if(!mem)
  {
    logger(LOGGER_INFO, ROVER_LCD16, "Memory not initialized");
    return 1;
  }

  logger(LOGGER_INFO, ROVER_LCD16, "LCD16 initialized");
  
  while(1)
  {
    if(_update == 1){
      memcpy(&lcd16, &mem->msg, sizeof(lcd16));
      logger(LOGGER_INFO, ROVER_LCD16, lcd16.command);
      //call command here      
      if (semaphore_lock(&sema) == 0)
      {
        lcd16_action_select(lcd16.command, strlen(lcd16.command));
        queue.queueType = 1;
        snprintf(queue.bData, sizeof(queue.bData) ,"$:%04d:%04d:%s:FFFF:#", LCD16_ID, (int)strlen(mem->status.lcd16_status.msg_line1), mem->status.lcd16_status.msg_line1);        
        queue_send(mem->queue_server_id, &queue, (int)strlen(mem->status.lcd16_status.msg_line1) + 1);
        semaphore_unlock(&sema);
      }
      _update = 0;
    }
    
    else{
      pause();
    }
  }
}

void update(int s)
{
  _update = 1;
}

void end_lcd16(int s)
{
  logger(LOGGER_INFO, ROVER_LCD16, "LCD16 unlaunched");
  exit(s);
}

void segfault(int s)
{
  int *p = 0x00;
  *p = 5;
}