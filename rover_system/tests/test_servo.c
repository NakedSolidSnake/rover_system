#include <stdio.h>
#include <stdlib.h>
#include <servo.h>

#define LEFT        29
#define RIGHT       118
#define CENTER      73

int main()
{
  SERVO_init();

  SERVO_setAngle(CENTER);

  for(int i = LEFT; i < RIGHT; i++)
  {
    SERVO_setAngle(i);
  }

  for(int i = RIGHT; i >= LEFT; i--)
  {
    SERVO_setAngle(i);
  }

  SERVO_setAngle(CENTER);  

  return EXIT_SUCCESS;
}

