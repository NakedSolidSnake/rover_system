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

#define ROVER_SERVO "ROVER_SERVO"

static int _update = 0;

static sema_t sema = {
        .id = -1,
        .sema_count = 1,
        .state = LOCKED,
        .master = SLAVE
    };


void update(int s);
void end_servo(int s);

int main()
{

  servo_st servo;
  MEM *mem = NULL;

  SERVO_init();

  semaphore_init(&sema, 1234);

  signal_register(update, SIGUSR1);
  signal_register(end_servo, SIGTERM);

  semaphore_init(&sema, 1234);

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

void update(int s)
{
  _update = 1;
}

void end_servo(int s)
{
  logger(LOGGER_INFO, ROVER_SERVO, "Servo Unlaunched");
  exit(s);
}
