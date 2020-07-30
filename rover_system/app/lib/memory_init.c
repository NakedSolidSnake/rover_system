#include <stdlib.h>
#include <string.h>
#include <app.h>
#include <log/log.h>

#define ROVER_MEMORY "ROVER_MEMORY"

int mem_init(void)
{
  int ret = -1;
  shm_t shm;
  MEM *mem = NULL;

  ret = sharedMemoryInit(&shm, SHM_PROCESSES_KEY, sizeof(MEM));
  if (ret)
  {
    return EXIT_FAILURE;
  }


  memset(shm.shm, 0, sizeof(MEM));

  mem = (MEM *)shm.shm;

  mem->init = 1;
  mem->shm.id = shm.id;
  mem->shm.shm = shm.shm;

  mem->monitor_enable = 1;

  logger(LOGGER_INFO, ROVER_MEMORY, "Memory initialized");  

  return EXIT_SUCCESS;
}


MEM *mem_get(void)
{
  MEM *mem = NULL;
  shm_t shm;
  int ret;

  ret = sharedMemoryInit(&shm, SHM_PROCESSES_KEY, sizeof(MEM));
  if (ret)
    return NULL;

  mem = (MEM *)shm.shm;

  if (!mem)
    return NULL;

  if (mem->init)
    return mem;

  return NULL;
}

int ipc_init(void)
{
  MEM *mem  = mem_get();
  if(!mem){
    return -1;
  }
  
  mem->sema_message.id = -1;
  mem->sema_message.sema_count = 1;
  mem->sema_message.state = LOCKED;
  mem->sema_message.master = MASTER;

  semaphore_init(&mem->sema_message, SEMA_MESSAGE_ID);
  if(mem->sema_message.id < 0)
  {
    logger(LOGGER_INFO, ROVER_MEMORY, "Semaphore Message init failed");
    exit(1);
  }

  mem->sema_update.id = -1;
  mem->sema_update.sema_count = 1;
  mem->sema_update.state = LOCKED;
  mem->sema_update.master = MASTER;

  semaphore_init(&mem->sema_update, SEMA_UPDATE_ID);
  if(mem->sema_update.id < 0)
  {
    logger(LOGGER_INFO, ROVER_MEMORY, "Semaphore Update init failed");
    exit(1);
  }

  mem->queueid = queue_init(QUEUE_MANAGER_ID);
  if (mem->queueid < 0)
  {
    logger(LOGGER_INFO, ROVER_MEMORY, "Queue Manager init failed");
    // exit(1);
    return -1;
  }

  mem->queue_server_id = queue_init(QUEUE_SERVER_ID);
  if (mem->queue_server_id < 0)
  {
    logger(LOGGER_INFO, ROVER_MEMORY, "Queue Server init failed");
    // exit(1);
    return -1;
  }

  return 0;
}

int mem_denit(void)
{
  MEM *mem = NULL;

  mem = mem_get();
  if(mem == NULL)
  {
    return 1;
  }

  queue_destroy(mem->queueid);  
  queue_destroy(mem->queue_server_id);
  sharedMemoryDestroy(&mem->shm);
  semaphore_destroy(&mem->sema_message);
  semaphore_destroy(&mem->sema_update);

  logger(LOGGER_INFO, ROVER_MEMORY, "Memory  unitialized");
  return 0;
}

int memoryWrite(MEM * mem, void *data, int offset)
{    
    process_st *pData = (process_st *)data;
    if(!data || !mem)
        return -1;
   
    memcpy(&mem->processes[offset], pData, sizeof(process_st));

    return 0;
}
