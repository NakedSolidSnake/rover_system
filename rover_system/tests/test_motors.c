#include <stdio.h>
#include <stdlib.h>
#include <motors.h>

int main()
{
  MOTORS_init();

  MOTORS_setPower(500); 
  MOTORS_setDirection(MOTOR_DIR_FORWARD);
  sleep(5);

  MOTORS_setPower(500); 
  MOTORS_setDirection(MOTOR_DIR_REVERSE);
  sleep(5);

  for(int i = 0; i <= 128; i++){
    MOTORS_incSpeed(i);
    usleep(1000 * 100);
  }


  for(int i = 128; i >= 0; i--){
    MOTORS_decSpeed(i);
    usleep(1000 * 100);
  }

  MOTORS_setPower(500); 
  MOTORS_setDirection(MOTOR_DIR_RIGHT);

  sleep(5);
  MOTORS_setPower(500); 
  MOTORS_setDirection(MOTOR_DIR_LEFT);

  sleep(5);


  MOTORS_setPower(0);
  MOTORS_setDirection(MOTOR_IDLE);

  return EXIT_SUCCESS;

}
