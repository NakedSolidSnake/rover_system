#include <includes.h>
#include <log/log.h>
#include <signal/signal.h>
#include <motors_action.h>
#include <motors.h>
#include <rover_motor_control.h>
#include <context.h>

#define ROVER_MOTOR   "ROVER_MOTOR"

static Context_st motor_context;

static void init(void);
static void message_received(int s);
static void update_time(int s);
static void end_motor(int s);


#ifdef PROCESS
int main(int argc, char const *argv[])
{
    (void)rover_motor_control(NULL);
    return 0;
}
#endif

void *rover_motor_control(void *args)
{
  (void)args;

  init();
  
  while(1)
  {
    if (motor_context.states.message_received)
    {
      memcpy(&motor_context.msg, &motor_context.mem->msg, sizeof(message_st));
      logger(LOGGER_INFO, ROVER_MOTOR, motor_context.msg.command);
      //call command here
      // if (semaphore_lock(&motor_context.sema_message) == 0)
      {
        motors_action_select(motor_context.msg.command, strlen(motor_context.msg.command));
        motor_context.queue.queueType = 1;
        snprintf(motor_context.queue.bData, sizeof(motor_context.queue.bData) ,"$:%04d:%04d:%s:FFFF:#",
                                                                               MOTOR_ID,
                                                                               (int)strlen(motor_context.msg.command),
                                                                               motor_context.msg.command);

        loggerArgs(LOGGER_INFO, ROVER_MOTOR, "MOTOR send: %s", motor_context.queue.bData);

        queue_send(motor_context.mem->queue_server_id, &motor_context.queue, (int)strlen(motor_context.queue.bData) + 1);
        // semaphore_unlock(&motor_context.sema_message);
      }

      motor_context.states.message_received = 0;
    }

    else if(motor_context.states.update_time){
      int index = get_pid(getpid());
      if(index >= 0 &&  index < motor_context.mem->process_amount)
      {
        clock_gettime(CLOCK_MONOTONIC, &motor_context.current);        
        // if (semaphore_lock(&motor_context.sema_update) == 0)
        {
          motor_context.mem->processes[index].update = (time_t)((double)motor_context.current.tv_sec + (double)motor_context.current.tv_nsec/(double)1000000000);
          // semaphore_unlock(&motor_context.sema_update);
        }
      }
      motor_context.states.update_time = 0;
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
  MOTORS_init();

  memset(&motor_context, 0, sizeof(Context_st));

  motor_context.mem = mem_get();
  if(!motor_context.mem)
  {
    logger(LOGGER_INFO, ROVER_MOTOR, "Memory not initialized");
    exit(EXIT_FAILURE);
  }

  motor_context.sema_message.id = -1;
  motor_context.sema_message.sema_count = 1;
  motor_context.sema_message.state = LOCKED;
  motor_context.sema_message.master = SLAVE;

  semaphore_init(&motor_context.sema_message, SEMA_MESSAGE_ID);

  motor_context.sema_update.id = -1;
  motor_context.sema_update.sema_count = 1;
  motor_context.sema_update.state = LOCKED;
  motor_context.sema_update.master = SLAVE;

  semaphore_init(&motor_context.sema_update, SEMA_UPDATE_ID);
  
  signal_register(message_received, SIGMESSAGERECEIVED);
  signal_register(update_time, SIGUPDATETIME);
  signal_register(update_time, SIGALRM);
  signal_register(end_motor, SIGTERM);

  logger(LOGGER_INFO, ROVER_MOTOR, "MOTORS initialized");
  alarm(PROCESS_CICLE_SECONDS);
}

void message_received(int s)
{  
  motor_context.states.message_received = 1;
}

static void update_time(int s)
{
  motor_context.states.update_time = 1;
}

void end_motor(int s)
{
  logger(LOGGER_INFO, ROVER_MOTOR, "MOTORS unlaunched");
  semaphore_unlock(&motor_context.sema_update);
  semaphore_unlock(&motor_context.sema_message);
  exit(s);
}
