#ifndef _ROVER_STATUS_H
#define _ROVER_STATUS_H

typedef struct motor_status
{
  int power;
  int direction;
}motor_status_t;

typedef struct Status
{
  motor_status_t m_status;
}status_t;

#endif
