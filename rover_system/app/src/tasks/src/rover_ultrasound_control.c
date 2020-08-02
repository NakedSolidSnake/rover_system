#include <includes.h>
#include <ultrasound_action.h>
#include <hc_sr04.h>
#include <rover_ultrasound_control.h>
#include <context.h>
#include <control_interface.h>

#define ROVER_ULTRASOUND   "ROVER_ULTRASOUND"

#ifdef PROCESS
int main(int argc, char const *argv[])
{
    (void)rover_ultrasound_control(NULL);
    return 0;
}
#endif

void *rover_ultrasound_control(void *args)
{
  (void)args;
  Context_st ultrasound_context = {0};
  ultrasound_context.logType = ROVER_ULTRASOUND;
  ultrasound_context.deviceId = ULTRASOUND_ID;

  IControl ultrasoundControl = 
  {
     .hardwareInit = HC_SR04_init, 
     .actionSelect = ultrasound_action_select
  };

  controlExec(&ultrasound_context, &ultrasoundControl);
  
  return NULL;
}