#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal/signal.h>
#include <signal.h>
#include <log/log.h>
#include <app.h>
#include <sharedmemory/sharedmemory.h>
#include <semaphore/semaphore.h>
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

  for(int i = 0; i < PROCESS_AMOUNT; i++)
  {
     
    if(mem->procs[i].pid != -1)
      notify_process(mem->procs[i].pid, SIGTERM);
  }

  queue_destroy(mem->queueid);  
  queue_destroy(mem->queue_server_id);

  sharedMemoryDestroy(&mem->shm);

  semaphore_destroy(&mem->sema);

  return EXIT_SUCCESS;
}
