#include <includes.h>
#include <lcd16_action.h>
#include <lcd16.h>
#include <rover_lcd16_control.h>
#include <context.h>
#include <control_interface.h>

#define ROVER_LCD16   "ROVER_LCD16"

#ifdef PROCESS
int main(int argc, char const *argv[])
{
    (void)rover_lcd16_control(NULL);
    return 0;
}
#endif

void *rover_lcd16_control(void *args)
{
  (void)args;
  Context_st lcd16_context = {0};
  lcd16_context.logType = ROVER_LCD16;
  lcd16_context.deviceId = LCD16_ID;

  IControl lcd16Control = 
  {
     .hardwareInit = LCD16_init, 
     .actionSelect = lcd16_action_select
  };

  controlExec(&lcd16_context, &lcd16Control);

  return NULL;
}