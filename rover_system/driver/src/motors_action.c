#include <motors_action.h>
#include <motors.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <app.h>

typedef struct tab_mov
{
  const char *mov;
  int (*cb_mov)(int sense);
  int sense;
} tab_mov_t;

static MEM *mem = NULL;


static const tab_mov_t movements[] =
{
  {.mov = MOVE_FORWARD, .cb_mov = MOTORS_setDirection, .sense = MOTOR_DIR_FORWARD},
  {.mov = MOVE_REVERSE, .cb_mov = MOTORS_setDirection, .sense = MOTOR_DIR_REVERSE},
  {.mov = MOVE_LEFT,    .cb_mov = MOTORS_setDirection, .sense = MOTOR_DIR_LEFT},
  {.mov = MOVE_RIGHT,   .cb_mov = MOTORS_setDirection, .sense = MOTOR_DIR_RIGHT},
  {.mov = MOVE_STOP,    .cb_mov = MOTORS_setDirection, .sense = MOTOR_IDLE}
};

static int movements_amount = sizeof(movements) / sizeof(movements[0]);

static int motors_action(const char *command);

int motors_action_select(const char *action, int action_len)
{
  char command[10] = {0};
  char data[10] = {0};
  char *p = NULL;
  int power = 0;
  int ret = 0;

  mem = mem_get();
  if(mem == NULL)
  {    
    return 1;
  }

  sscanf(action, "%s", command);

  if (!strncmp(command, MOVE, strlen(MOVE)))
  {
    p = (char *)(action + strlen(command) + 1); //points to next string
    strncpy(data, p, 10);
    //call functions    
    ret = motors_action(p);
  }
  else if (!strncmp(command, POWER, strlen(POWER)))
  {
    sscanf(action, "%s %d", data, &power);
    //call function
    mem->status.motor_status.power = power;
    ret = MOTORS_setPower(power);
  }
  
  return ret;
}

static int motors_action(const char *command)
{
  int ret = 1;

  for (int i = 0; i < movements_amount; i++)
  {
    tab_mov_t *comm = (tab_mov_t *)&movements[i];
    if (!strncmp(command, comm->mov, strlen(comm->mov)))
    {
      mem->status.motor_status.direction = comm->sense;
      ret = comm->cb_mov(comm->sense);
      mem->status.motor_status.direction = comm->sense;
      break;
    }
  }
  return ret;
}
