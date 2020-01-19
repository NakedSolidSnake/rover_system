#include <motors.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct motors_t{
  int power;
  int range;
  int _clock;
}motors_t;

static void forwardSense(void);
static void reverseSense(void);
static void turnLetf(void);
static void turnRight(void);
static void idle(void);


int MOTORS_init()
{
  printf("MOTORS initialized.\n");
  return EXIT_SUCCESS;
}

int MOTORS_setPower(int power)
{
  printf("Power: %d\n", power);
  return 0;
}

int MOTORS_setDirection(int direction)
{
  switch(direction)
  {
    case MOTOR_DIR_FORWARD:
      forwardSense();
      break;

    case MOTOR_DIR_REVERSE:
      reverseSense();
      break;

    case MOTOR_DIR_RIGHT:
      turnRight();
      break;

    case MOTOR_DIR_LEFT:
      turnLetf();
      break;

    case MOTOR_IDLE:
      idle();
      break;

    default:
      return -1;
  }

  return 0;

} 

int MOTORS_incSpeed(int speedRate)
{
  printf("Motor speed : %d\n", speedRate);
  return 0;
}

int MOTORS_decSpeed(int speedRate)
{
  printf("Motor speed : %d\n", speedRate);
  return 0;
}

static void forwardSense(void)
{
  printf("Motor forward.\n");
}

static void reverseSense(void)
{
  printf("Motor reverse.\n");
}

static void turnLetf(void)
{
  printf("Motor left.\n");
}

static void turnRight(void)
{
  printf("Motor right.\n");
}

static void idle(void)
{
  printf("Motor idle.\n");
}
