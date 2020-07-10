#ifndef _ROVER_STATUS_H
#define _ROVER_STATUS_H

#define LCD16_LINES     16

typedef enum MOTOR_Direction
{
  MOTOR_Dir_Forward  = 0,
  MOTOR_Dir_Reverse, 
  MOTOR_Dir_Right,   
  MOTOR_Dir_Left ,   
  MOTOR_Idle,        
}MOTOR_Direction_e;

typedef struct motor_status
{
  int power;
  int direction;  
}motor_status_st;

typedef struct servo_status
{
  int position;
}servo_status_st;

typedef struct ultrasound_status
{
  float distance;
}ultrasound_status_st;

typedef struct lcd16_status
{
  char msg_line1[LCD16_LINES];
  char msg_line2[LCD16_LINES];
}lcd16_status_st;

typedef struct Status
{  
  motor_status_st motor_status;
  servo_status_st servo_status;
  ultrasound_status_st ultrasound_status;
  lcd16_status_st lcd16_status;
}Status_st;

#endif
