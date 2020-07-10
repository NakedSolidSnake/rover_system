#include <servo_action.h>
#include <servo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <app.h>

static MEM *mem = NULL;

typedef struct tab_mov
{
  const char *mov;
  int (*cb_mov)(int position);
  int position;
} tab_mov_t;

static const tab_mov_t movements[] =
    {
        {.mov = MOVE_CENTER, .cb_mov = SERVO_setAngle, .position = CENTER},
        {.mov = MOVE_LEFT, .cb_mov = SERVO_setAngle, .position = LEFT},
        {.mov = MOVE_RIGHT, .cb_mov = SERVO_setAngle, .position = RIGHT},
};

static int movements_amount = sizeof(movements) / sizeof(movements[0]);

static int servo_action(const char *command);

int servo_action_select(const char *action, int action_len)
{
  char command[10] = {0};
  char data[10] = {0};
  char *p = NULL;
  int position = 0;
  int ret = 0;

  mem = mem_get();
  if(mem == NULL)
  {    
    return 1;
  }

  sscanf(action, "%s", command);

  if (!strncmp(command, SET, strlen(SET)))
  {
    p = (char *)(action + strlen(command) + 1); //points to next string
    strncpy(data, p, 10);
    //call functions
    ret = servo_action(p);
  }
  else if (!strncmp(command, GRAUS, strlen(GRAUS)))
  {
    sscanf(action, "%s %d", data, &position);
    //call function
    ret = SERVO_setAngle(position);
    mem->status.servo_status.position = position;
  }

  return ret;
}

static int servo_action(const char *command)
{
  int ret = 1;

  for (int i = 0; i < movements_amount; i++)
  {
    tab_mov_t *comm = (tab_mov_t *)&movements[i];
    if (!strncmp(command, comm->mov, strlen(comm->mov)))
    {
      ret = comm->cb_mov(comm->position);
      break;
    }
  }
  return ret;
}
