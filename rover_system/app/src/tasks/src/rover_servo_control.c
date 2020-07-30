#include <includes.h>
#include <log/log.h>
#include <signal/signal.h>
#include <servo_action.h>
#include <servo.h>
#include <rover_servo_control.h>
#include <context.h>

#define ROVER_SERVO "ROVER_SERVO"

static Context_st servo_context;

static void init(void);
static void message_received(int s);
static void update_time(int s);
static void end_servo(int s);




#ifdef PROCESS
int main(int argc, char const *argv[])
{
    (void)rover_servo_control(NULL);
    return 0;
}
#endif

void *rover_servo_control(void *args)
{
  (void)args;

  init();

  while(1)
  {
    if(servo_context.states.message_received)
    {
      memcpy(&servo_context.msg, &servo_context.mem->msg, sizeof(message_st));
      logger(LOGGER_INFO, ROVER_SERVO, servo_context.msg.command);
      //call command here
      
      // if (semaphore_lock(&servo_context.sema_message) == 0)
      {
        servo_action_select(servo_context.msg.command, strlen(servo_context.msg.command));
        servo_context.queue.queueType = 1;
        snprintf(servo_context.queue.bData, sizeof(servo_context.queue.bData) ,"$:%04d:%04d:%s:FFFF:#",
                                                                               SERVO_ID,
                                                                               (int)strlen(servo_context.msg.command),
                                                                               servo_context.msg.command);
        queue_send(servo_context.mem->queue_server_id, &servo_context.queue, (int)strlen(servo_context.queue.bData) + 1);
        // semaphore_unlock(&servo_context.sema_message);
      }
      servo_context.states.message_received = 0;
    }

    else if(servo_context.states.update_time){
      int index = get_pid(getpid());
      if(index >= 0 &&  index < servo_context.mem->process_amount)
      {
        clock_gettime(CLOCK_MONOTONIC, &servo_context.current);        
        // if (semaphore_lock(&servo_context.sema_update) == 0)
        {
          servo_context.mem->processes[index].update = (time_t)((double)servo_context.current.tv_sec + (double)servo_context.current.tv_nsec/(double)1000000000);
          // semaphore_unlock(&servo_context.sema_update);
        }
      }
      servo_context.states.update_time = 0;
      alarm(PROCESS_CICLE_SECONDS);
    }

    else
    {
      pause();
    }
  }
  return NULL;
}

static void init(void)
{
  SERVO_init();

  memset(&servo_context, 0, sizeof(Context_st));

  servo_context.mem = mem_get();
  if(!servo_context.mem)
  {
    logger(LOGGER_INFO, ROVER_SERVO, "Memory not initialized");
    exit(EXIT_FAILURE);
  }

  servo_context.sema_message.id = -1;
  servo_context.sema_message.sema_count = 1;
  servo_context.sema_message.state = LOCKED;
  servo_context.sema_message.master = SLAVE;

  semaphore_init(&servo_context.sema_message, SEMA_MESSAGE_ID);

  servo_context.sema_update.id = -1;
  servo_context.sema_update.sema_count = 1;
  servo_context.sema_update.state = LOCKED;
  servo_context.sema_update.master = SLAVE;

  semaphore_init(&servo_context.sema_update, SEMA_UPDATE_ID);
  
  signal_register(message_received, SIGMESSAGERECEIVED);
  signal_register(update_time, SIGUPDATETIME);
  signal_register(update_time, SIGALRM);
  signal_register(end_servo, SIGTERM);

  logger(LOGGER_INFO, ROVER_SERVO, "SERVO initialized");
  alarm(PROCESS_CICLE_SECONDS);
}

void message_received(int s)
{  
  servo_context.states.message_received = 1;
}

static void update_time(int s)
{
  servo_context.states.update_time = 1;
}

void end_servo(int s)
{
  logger(LOGGER_INFO, ROVER_SERVO, "SERVO unlaunched");
  semaphore_unlock(&servo_context.sema_update);
  semaphore_unlock(&servo_context.sema_message);
  exit(s);
}
