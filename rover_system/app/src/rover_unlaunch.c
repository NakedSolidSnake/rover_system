#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal/signal.h>
#include <signal.h>
#include <log/log.h>
#include <app.h>
#include <sharedmemory/sharedmemory.h>
#include <queue/queue.h>

#define ROVER_UNLAUNCH "ROVER_UNLAUNCH"

int main()
{  
  MEM *mem = NULL;

  mem = mem_get();
  if(mem == NULL)
  {
    logger(LOGGER_INFO, ROVER_UNLAUNCH, "Memory not initialized");
    return 1;
  }

  for(int i = 0; i < 4; i++)
  {
     
    if(mem->procs[i].pid != -1)
      notify_process(mem->procs[i].pid, SIGTERM);
  }

  queue_destroy(mem->queueid);  

  sharedMemoryDestroy(&mem->shm);

  return EXIT_SUCCESS;
}
