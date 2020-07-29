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

typedef struct processList
{ 
    char name[PROC_NAME_MAX];    
}processList_st;

static void init_fail(MEM *mem, int amount);

static processList_st pList[] = 
{
        {.name = ROVER_PROCESS_MOTOR     },
        {.name = ROVER_PROCESS_SERVO     },
        {.name = ROVER_PROCESS_MANAGER   },
        {.name = ROVER_PROCESS_SERVER    },
        {.name = ROVER_PROCESS_MQTT      },
        {.name = ROVER_PROCESS_WEBSERVER },
        {.name = ROVER_PROCESS_ULTRASOUND},
        {.name = ROVER_PROCESS_LCD16     },
        {.name = ROVER_PROCESS_MONITOR   }
};

static int processes_amount = sizeof(pList)/sizeof(pList[0]);

int main()
{
  // int ret;
  MEM *mem  = NULL;

  if(mem_init())
    return -1;

  if(ipc_init() < 0)
    return -1;

  mem = mem_get();
  if(!mem)
    return  -1;

  for (int i = 0; i < processes_amount; i++)
    {
        pid_t pid = fork();
        if(pid == 0){
            execl(pList[i].name, pList[i].name, NULL);
        } 
        else if(pid > 0){
            process_st process;
            memset(&process, 0, sizeof(process_st));
            process.pid = pid;            
            strncpy(process.name, pList[i].name, strlen(pList[i].name) + 1);
            memoryWrite(mem, (void *)&process, i);
        }
        else{
            init_fail(mem, PROCESS_AMOUNT);
            exit(1);
        }
    }

  mem->process_amount = processes_amount;
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
    process_st *proc = &(mem->processes[i]);
    if (proc->pid != -1)
    {
      notify_process(proc->pid, SIGKILL);
    }
  }
  exit(1);
}
