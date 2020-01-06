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

#define ROVER_SERVO "ROVER_SERVO"

static int _update = 0;

void update(int s);
void end_servo(int s);

int main()
{

  servo_st servo;
  MEM *mem = NULL;

  SERVO_init();

  signal_register(update, SIGUSR1);
  signal_register(end_servo, SIGTERM);

  mem = mem_get();
  if(!mem)
  {
    log(LOG_INFO, ROVER_SERVO, "Memory not initialized");
    return 1;
  }

  while(1)
  {
    if(_update == 1)
    {
      memcpy(&servo, &mem->servo, sizeof(servo));
      log(LOG_INFO, ROVER_SERVO, servo.command);
      //call command here
      servo_action_select(servo.command, strlen(servo.command));
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
  log(LOG_INFO, ROVER_SERVO, "Servo Unlaunched");
  exit(s);
}
