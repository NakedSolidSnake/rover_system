#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <rover_types.h>
#include <log/log.h>
#include <signal/signal.h>
#include <signal.h>
#include <motors_action.h>
#include <motors.h>
#include <app.h>
#include <semaphore/semaphore.h>
#include <queue/queue.h>

#define ROVER_MOTOR   "ROVER_MOTOR"

static int _update = 0;

void update(int s);
void end_motor(int s);

static sema_t sema = {
        .id = -1,
        .sema_count = 1,
        .state = LOCKED,
        .master = SLAVE
    };

int main()
{
  queue_st queue;
  motor_st motores;
  MEM *mem = NULL;

  MOTORS_init();

  semaphore_init(&sema, SEMA_ID);

  signal_register(update, SIGUSR1);
  signal_register(end_motor, SIGTERM);

  mem = mem_get();
  if(!mem)
  {
    logger(LOGGER_INFO, ROVER_MOTOR, "Memory not initialized");
    return 1;
  }

  logger(LOGGER_INFO, ROVER_MOTOR, "Motor initialized");
  
  while(1)
  {
    if (_update == 1)
    {
      memcpy(&motores, &mem->motor, sizeof(motores));
      logger(LOGGER_INFO, ROVER_MOTOR, motores.command);
      //call command here
      if (semaphore_lock(&sema) == 0)
      {
        motors_action_select(motores.command, strlen(motores.command));
        queue.queueType = 1;
        snprintf(queue.bData, sizeof(queue.bData) ,"$:%04d:%04d:%s:FFFF:#", MOTOR_ID, (int)strlen(motores.command), motores.command);
        queue_send(mem->queue_server_id, &queue, (int)strlen(motores.command) + 1);
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

void end_motor(int s)
{
  logger(LOGGER_INFO, ROVER_MOTOR, "Motor unlaunched");
  exit(s);
}