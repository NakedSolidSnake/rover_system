#include <app.h>
#include <sharedmemory/sharedmemory.h>
#include <stdlib.h>
#include <string.h>


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
  int ret;

  MEM *mem  = mem_get();
  if(!mem){
    return -1;
  }
  
  mem->sema.id = -1;
  mem->sema.sema_count = 1;
  mem->sema.state = LOCKED;
  mem->sema.master = MASTER;

  semaphore_init(&mem->sema, SEMA_ID);
  if(mem->sema.id < 0)
  {
    // logger(LOGGER_INFO, ROVER_LAUNCH, "Semaphore init failed");
    // exit(1);
    return -1;
  }

  mem->queueid = queue_init(QUEUE_MANAGER_ID);
  if (mem->queueid < 0)
  {
    // logger(LOGGER_INFO, ROVER_LAUNCH, "Queue Manager init failed");
    // exit(1);
    return -1;
  }

  mem->queue_server_id = queue_init(QUEUE_SERVER_ID);
  if (mem->queue_server_id < 0)
  {
    // logger(LOGGER_INFO, ROVER_LAUNCH, "Queue Server init failed");
    // exit(1);
    return -1;
  }

  // logger(LOGGER_INFO, ROVER_LAUNCH, "Queue Initialized");

  // ret = mem_init(&mem);
  // if (ret)
  // {
  //   // logger(LOGGER_INFO, ROVER_LAUNCH, "Error to create process table");
  //   return -1;
  // }

  // logger(LOGGER_INFO, ROVER_LAUNCH, "Memory Initialized");

  return 0;
}

int ipc_denit(void)
{

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
  semaphore_destroy(&mem->sema);
}

int memoryWrite(MEM * mem, void *data, int offset)
{    
    process_st *pData = (process_st *)data;
    if(!data || !mem)
        return -1;
   
    memcpy(&mem->processes[offset], pData, sizeof(process_st));

    return 0;
}