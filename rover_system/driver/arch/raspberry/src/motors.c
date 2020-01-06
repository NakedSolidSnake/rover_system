#include <motors.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

typedef struct motors_t{
  int power;
  int range;
  int _clock;
}motors_t;

static motors_t m = {
  .power = 0,
  .range = 1000,
  ._clock = 384 
};


static void forwardSense(void);
static void reverseSense(void);
static void turnLetf(void);
static void turnRight(void);
static void idle(void);


int MOTORS_init()
{
  int ret = wiringPiSetupGpio();
  if(ret < 0){
    return -1;
  }

  pinMode(MOTOR1_POWER, PWM_OUTPUT);
  pinMode(MOTOR2_POWER, PWM_OUTPUT);
  pwmSetMode(PWM_MODE_MS);
  pwmSetRange(m.range);
  pwmSetClock(m._clock);

  MOTORS_setPower(m.power);

  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);

  //set forward sense


  return EXIT_SUCCESS;
}

int MOTORS_setPower(int power)
{
  if(power > m.range)
    m.power  = m.range;

  if(power < 0)
    m.power = 0;

  m.power = power;

  pwmWrite(MOTOR1_POWER, power);
  pwmWrite(MOTOR2_POWER, power);
  usleep(5000);
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
  m.power += speedRate;
  MOTORS_setPower(m.power);

  return 0;
}

int MOTORS_decSpeed(int speedRate)
{
  m.power -= speedRate;
  MOTORS_setPower(m.power);
  return 0;
}

static void forwardSense(void)
{
  //motor 1 sense
  digitalWrite(MOTOR1_IN1, HIGH);
  digitalWrite(MOTOR1_IN2, LOW);

  //motor 2 sense
  digitalWrite(MOTOR2_IN1, HIGH);
  digitalWrite(MOTOR2_IN2, LOW);
}

static void reverseSense(void)
{
  //motor 1 sense
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, HIGH);

  //motor 2 sense
  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, HIGH);
}

static void turnLetf(void)
{
  //motor 1 sense
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, LOW);

  //motor 2 sense
  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, HIGH);
  
}

static void turnRight(void)
{

  //motor 1 sense
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, HIGH);

  //motor 2 sense
  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, LOW);
}

static void idle(void)
{
  //motor 1 sense
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, LOW);

  //motor 2 sense
  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, LOW);
}
