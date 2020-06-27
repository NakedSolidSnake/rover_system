#ifndef __MANAGER_H
#define __MANAGER_H

#include <app.h>

typedef enum TYPE{
  MOTOR_ID = 0,
  SERVO_ID,
  ULTRASOUND_ID,
  LCD16_ID
}TYPE_e;

int manager(int id, const char *command, MEM *mem);

#endif
