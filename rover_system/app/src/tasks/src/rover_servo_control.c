#include <includes.h>
#include <servo_action.h>
#include <servo.h>
#include <rover_servo_control.h>
#include <context.h>
#include <control_interface.h>

#define ROVER_SERVO "ROVER_SERVO"

#ifdef PROCESS
int main(int argc, char const *argv[])
{
    (void)rover_servo_control(NULL);
    return 0;
}
#endif

void *rover_servo_control(void *args)
{
  (void)args;
  Context_st servo_context = {0};
  servo_context.logType = ROVER_SERVO;
  servo_context.deviceId = MOTOR_ID;

  IControl servoControl = 
  {
     .hardwareInit = SERVO_init, 
     .actionSelect = servo_action_select
  };

 controlExec(&servo_context, &servoControl);

 return NULL;
}