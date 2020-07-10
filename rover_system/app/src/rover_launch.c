#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <log/log.h>
#include <sharedmemory/sharedmemory.h>
#include <queue/queue.h>
#include <signal/signal.h>
#include <semaphore/semaphore.h>
#include <app.h>

#define BBSIZE       128

#define ROVER_LAUNCH "ROVER_LAUNCH"

static void init_fail(MEM *mem, int amount);

int main()
{
  int ret;
  MEM mem ;

  memset(&mem, 0, sizeof(MEM));

  mem.sema.id = -1;
  mem.sema.sema_count = 1;
  mem.sema.state = LOCKED;
  mem.sema.master = MASTER;

  semaphore_init(&mem.sema, SEMA_ID);
  if(mem.sema.id < 0)
  {
    logger(LOGGER_INFO, ROVER_LAUNCH, "Semaphore init failed");
    exit(1);
  }

  mem.queueid = queue_init(QUEUE_MANAGER_ID);
  if (mem.queueid < 0)
  {
    logger(LOGGER_INFO, ROVER_LAUNCH, "Queue init failed");
    exit(1);
  }

  logger(LOGGER_INFO, ROVER_LAUNCH, "Queue Initialized");

  ret = mem_init(&mem);
  if (ret)
  {
    logger(LOGGER_INFO, ROVER_LAUNCH, "Error to create process table");
    return -1;
  }

  logger(LOGGER_INFO, ROVER_LAUNCH, "Memory Initialized");
 

  for (int i = 0; i < (PROCESS_AMOUNT); i++)
  {
    process_t *proc = &mem.procs[i];
    proc->pid = fork();
    if (proc->pid == 0)
    {
      char buffer[BBSIZE] = {0};
      snprintf(buffer, sizeof(buffer), "%s pid[%d]", proc->name, getpid());
      logger(LOGGER_INFO, ROVER_LAUNCH, buffer);
      execv(proc->name, (char *)NULL);
    }
    else if (proc->pid == -1)
    {
      init_fail(&mem, PROCESS_AMOUNT);
    }
    else
    {
      continue;
    }
  }  
  mem_set(&mem);
  return EXIT_SUCCESS;
}

static void init_fail(MEM *mem, int amount)
{
  if (!mem)
  {
    logger(LOGGER_INFO, ROVER_LAUNCH, "Shared Memory not initialized");
    return;
  }

  for (int i = 0; i < amount; i++)
  {
    process_t *proc = &(mem->procs[i]);
    if (proc->pid != -1)
    {
      notify_process(proc->pid, SIGKILL);
    }
  }
  exit(1);
}
