#ifndef _MOTORS_H
#define _MOTORS_H

#define MOTOR1_POWER   12
#define MOTOR1_IN1     16
#define MOTOR1_IN2     20

#define MOTOR2_POWER   13 
#define MOTOR2_IN1     19 
#define MOTOR2_IN2     26

#define MOTOR_DIR_FORWARD 0
#define MOTOR_DIR_REVERSE 1
#define MOTOR_DIR_RIGHT   2
#define MOTOR_DIR_LEFT    3
#define MOTOR_IDLE        4

int MOTORS_init();
int MOTORS_setPower(int power);
int MOTORS_setDirection(int direction);
int MOTORS_incSpeed(int speedRate);
int MOTORS_decSpeed(int speedRate);

#endif 
