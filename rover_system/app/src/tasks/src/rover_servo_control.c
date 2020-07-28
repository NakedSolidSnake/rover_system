#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <rover_types.h>
#include <log/log.h>
#include <signal/signal.h>
#include <signal.h>
#include <servo_action.h>
#include <servo.h>
#include <app.h>
#include <semaphore/semaphore.h>
#include <queue/queue.h>
#include <rover_servo_control.h>

#define ROVER_SERVO "ROVER_SERVO"

static int _update = 0;

static sema_t sema = {
        .id = -1,
        .sema_count = 1,
        .state = LOCKED,
        .master = SLAVE
    };


static void update(int s);
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
   queue_st queue;
  servo_st servo;
  MEM *mem = NULL;

  SERVO_init();

  semaphore_init(&sema, SEMA_ID);

  signal_register(update, SIGUSR1);
  signal_register(end_servo, SIGTERM);

  mem = mem_get();
  if(!mem)
  {
    logger(LOGGER_INFO, ROVER_SERVO, "Memory not initialized");
    return 1;
  }

  logger(LOGGER_INFO, ROVER_SERVO, "Servo initialized");

  while(1)
  {
    if(_update == 1)
    {
      memcpy(&servo, &mem->servo, sizeof(servo));
      logger(LOGGER_INFO, ROVER_SERVO, servo.command);
      //call command here
      
      if (semaphore_lock(&sema) == 0)
      {
        servo_action_select(servo.command, strlen(servo.command));
        queue.queueType = 1;
        snprintf(queue.bData, sizeof(queue.bData) ,"$:%04d:%04d:%s:FFFF:#", SERVO_ID, (int)strlen(servo.command), servo.command);
        queue_send(mem->queue_server_id, &queue, (int)strlen(servo.command) + 1);
        semaphore_unlock(&sema);
      }
      _update = 0;
    }
    else
    {
      pause();
    }
  }
}

static void update(int s)
{
  _update = 1;
}

static void end_servo(int s)
{
  logger(LOGGER_INFO, ROVER_SERVO, "Servo Unlaunched");
  exit(s);
}
