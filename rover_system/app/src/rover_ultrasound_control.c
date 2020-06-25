#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <rover_types.h>
#include <log/log.h>
#include <signal/signal.h>
#include <signal.h>
#include <ultrasound_action.h>
#include <hc_sr04.h>
#include <app.h>

#define ROVER_ULTRASOUND   "ROVER_ULTRASOUND"

static int _update = 0;

void update(int s);
void end_ultrasound(int s);


int main()
{

  ultrasound_st ultrasound;
  MEM *mem = NULL;

  HC_SR04_init();

  signal_register(update, SIGUSR1);
  signal_register(end_ultrasound, SIGTERM);

  mem = mem_get();
  if(!mem)
  {
    logger(LOGGER_INFO, ROVER_ULTRASOUND, "Memory not initialized");
    return 1;
  }

  logger(LOGGER_INFO, ROVER_ULTRASOUND, "Ultrasound initialized");
  
  while(1)
  {
    if(_update == 1){
      memcpy(&ultrasound, &mem->ultrasound, sizeof(ultrasound));
      logger(LOGGER_INFO, ROVER_ULTRASOUND, ultrasound.command);
      //call command here
      ultrasound_action_select(ultrasound.command, strlen(ultrasound.command));
      _update = 0;
    } 
    else{
      pause();
    }
  }
}

void update(int s)
{
  _update = 1;
}

void end_ultrasound(int s)
{
  logger(LOGGER_INFO, ROVER_ULTRASOUND, "Ultrasound unlaunched");
  exit(s);
}