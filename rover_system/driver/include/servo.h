#ifndef __SERVO_H
#define __SERVO_H

#define SERVO_PIN   18
#define PWM_RANGE   1000
#define PWM_CLOCK   384

int SERVO_init(void);
int SERVO_setAngle(int angle);

#endif
