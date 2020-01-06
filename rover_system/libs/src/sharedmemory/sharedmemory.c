#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sharedmemory/sharedmemory.h>

int sharedMemoryInit(shm_t *shm, int key, int dataSize)
{
  if(!shm)
    return EXIT_FAILURE;

  shm->id = shmget((key_t)key, dataSize, 0666 | IPC_CREAT);
  if(shm->id == -1)
    return EXIT_FAILURE;

  shm->shm = shmat(shm->id, (void *)0, 0);
  if(!shm->shm)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int sharedMemoryWrite(shm_t *shm, void *buff, int offset, int size)
{
  if(!shm)
    return -1;

  if(offset < 0)
    return -1;

  if(size <= 0)
    return -1;

  memcpy((shm->shm + offset), buff, size);
  return 0;
}

int sharedMemoryRead(shm_t *shm, void *buff, int offset, int size)
{
  if(!shm)
    return -1;

  // if(!buff)
  //   return -1;

  if(offset < 0)
    return -1;

  if(size <= 0)
    return -1;

  memcpy(buff, (shm + offset), size);
  return 0;
}

int sharedMemoryDestroy(shm_t *shm)
{
  if(!shm)
    return -1;
  
  shmdt(shm->shm);
  return shmctl(shm->id, IPC_RMID, NULL);
}
