#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore/semaphore.h>

union semun{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
};


struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, 1, SEM_UNDO};


int semaphore_init(sema_t *s, int key)
{
  if(s == NULL)
    return -1;

  s->id = semget((key_t) key, 1, 0666 | IPC_CREAT);
  if(s->id < 0)
    return -1;

  union semun u;
  u.val = 1;

  if(semctl(s->id, 0, SETVAL, u) < 0)
    return -1;

  return 0;
}

int semaphore_lock(sema_t *s)
{

  if(s == NULL)
    return -1;

  if(semop(s->id, &p, 1) < 0)
    return -1;

  return 0;
}

int semaphore_unlock(sema_t *s)
{
  if(s == NULL)
    return -1;

  if(semop(s->id, &v, 1) < 0)
    return -1;

  return 0;
}

int semaphore_destroy(sema_t *s)
{
  if(s == NULL)
    return -1;

  if(semctl(s->id, 0, IPC_RMID) < 0)
    return -1;

  return 0;

}
