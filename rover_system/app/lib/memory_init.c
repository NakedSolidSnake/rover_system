#include <app.h>
#include <sharedmemory/sharedmemory.h>
#include <stdlib.h>
#include <string.h>

process_t procs[] =
    {
        {.pid = -1, .name = ROVER_PROCESS_MOTOR},
        {.pid = -1, .name = ROVER_PROCESS_SERVO},
        {.pid = -1, .name = ROVER_PROCESS_MANAGER},
        {.pid = -1, .name = ROVER_PROCESS_SERVER}};

int mem_init(MEM *mem)
{
  int ret = -1;
  shm_t shm;

  ret = sharedMemoryInit(&shm, SHM_PROCESSES_KEY, sizeof(MEM));
  if (ret)
  {
    return EXIT_FAILURE;
  }

  if (!mem->init)
  {
    mem->shm = shm;
    mem->procs[0] = procs[0];
    mem->procs[1] = procs[1];
    mem->procs[2] = procs[2];
    mem->procs[3] = procs[3];

    mem->init = 1;

    if (sharedMemoryWrite(&mem->shm, (void *)mem, 0, sizeof(MEM)))
    {
      return EXIT_FAILURE;
    }
  }

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

int mem_set(MEM *mem)
{
  if (!mem)
  {
    return EXIT_FAILURE;
  }

  if (sharedMemoryWrite(&mem->shm, (void *)mem, 0, sizeof(MEM)))
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
