#include <includes.h>
#include <motors_action.h>
#include <motors.h>
#include <rover_motor_control.h>
#include <context.h>
#include <control_interface.h>

#define ROVER_MOTOR   "ROVER_MOTOR"

#ifdef PROCESS
int main(int argc, char const *argv[])
{
    (void)rover_motor_control(NULL);
    return 0;
}
#endif

void *rover_motor_control(void *args)
{
  (void)args;
  Context_st motor_context = {0};
  motor_context.logType = ROVER_MOTOR;
  motor_context.deviceId = MOTOR_ID;

  IControl motorControl = 
  {
     .hardwareInit = MOTORS_init, 
     .actionSelect = motors_action_select
  };

 controlExec(&motor_context, &motorControl);

  return NULL;
}