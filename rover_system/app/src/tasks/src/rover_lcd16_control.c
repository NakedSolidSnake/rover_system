#include <includes.h>
#include <log/log.h>
#include <signal/signal.h>
#include <lcd16_action.h>
#include <lcd16.h>
#include <rover_lcd16_control.h>
#include <context.h>
#include <auxiliar.h>

#define ROVER_LCD16   "ROVER_LCD16"

static Context_st lcd16_context;


static void init(void);
static void message_received(int s);
static void update_time(int s);
static void end_lcd16(int s);

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
  
  init();

   
  while(1)
  {
    if(lcd16_context.states.message_received){
      memcpy(&lcd16_context.msg, &lcd16_context.mem->msg, sizeof(message_st));
      logger(LOGGER_INFO, ROVER_LCD16, lcd16_context.msg.command);
      //call command here      
      // if (semaphore_lock(&lcd16_context.sema_message) == 0)
      {
        lcd16_action_select(lcd16_context.msg.command, strlen(lcd16_context.msg.command));
        lcd16_context.queue.queueType = 1;
        snprintf(lcd16_context.queue.bData, sizeof(lcd16_context.queue.bData) ,"$:%04d:%04d:%s:FFFF:#",
                                                                              LCD16_ID,
                                                                              (int)strlen(lcd16_context.msg.command),
                                                                               lcd16_context.msg.command);        

        queue_send(lcd16_context.mem->queue_server_id, &lcd16_context.queue, (int)strlen(lcd16_context.queue.bData) + 1);
        // semaphore_unlock(&lcd16_context.sema_message);
      }
      lcd16_context.states.message_received = 0;
    } 

    else if(lcd16_context.states.update_time){
      update_clock(getpid(), &lcd16_context);
      alarm(PROCESS_CICLE_SECONDS);
    }
    
    else{
      pause();
    }
  }

  return NULL;
}

static void init(void)
{
  LCD16_init();

  memset(&lcd16_context, 0, sizeof(Context_st));

  lcd16_context.mem = mem_get();
  if(!lcd16_context.mem)
  {
    logger(LOGGER_INFO, ROVER_LCD16, "Memory not initialized");
    exit(EXIT_FAILURE);
  }

  lcd16_context.sema_message.id = -1;
  lcd16_context.sema_message.sema_count = 1;
  lcd16_context.sema_message.state = LOCKED;
  lcd16_context.sema_message.master = SLAVE;

  semaphore_init(&lcd16_context.sema_message, SEMA_MESSAGE_ID);

  lcd16_context.sema_update.id = -1;
  lcd16_context.sema_update.sema_count = 1;
  lcd16_context.sema_update.state = LOCKED;
  lcd16_context.sema_update.master = SLAVE;

  semaphore_init(&lcd16_context.sema_update, SEMA_UPDATE_ID);
  
  signal_register(message_received, SIGMESSAGERECEIVED);
  signal_register(update_time, SIGUPDATETIME);
  signal_register(update_time, SIGALRM);
  signal_register(end_lcd16, SIGTERM);

  logger(LOGGER_INFO, ROVER_LCD16, "Initialized");

  alarm(PROCESS_CICLE_SECONDS);
}

void message_received(int s)
{  
  lcd16_context.states.message_received = 1;
}

static void update_time(int s)
{
  lcd16_context.states.update_time = 1;
}

void end_lcd16(int s)
{
  logger(LOGGER_INFO, ROVER_LCD16, "LCD16 unlaunched");
  semaphore_unlock(&lcd16_context.sema_update);
  semaphore_unlock(&lcd16_context.sema_message);
  exit(s);
}
