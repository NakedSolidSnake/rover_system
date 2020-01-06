#include <stdio.h>
#include <stdlib.h>
#include <servo.h>

int SERVO_init(void)
{
  printf("Servo initialized.\n");
  return EXIT_SUCCESS;
}

int SERVO_setAngle(int angle)
{
  printf("Servo angle.\n");
  return EXIT_SUCCESS;
}
