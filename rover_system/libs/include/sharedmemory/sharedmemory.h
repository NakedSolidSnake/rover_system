#ifndef __SHAREDMEMORY_H
#define __SHAREDMEMORY_H

typedef struct shm_t
{
  int id;
  void *shm;
}shm_t;

int sharedMemoryInit(shm_t *shm, int key, int dataSize);
int sharedMemoryWrite(shm_t *shm, void *buff, int offset, int size);
int sharedMemoryRead(shm_t *shm, void *buff, int offset, int size);
int sharedMemoryDestroy(shm_t *shm);

#endif
