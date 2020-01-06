#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sharedmemory/sharedmemory.h>
#include <app.h>

#define ARGS_NO     0
#define ARGS_REQ    1
#define ARGS_OPT    2

typedef enum opt_e
{
  OPT_HELP = 0,
  OPT_PROCESS,
  OPT_SHM, 
  OPT_MOTOR,
  OPT_SERVO,
  OPT_QUEUE
}opt_et;

struct table  
{
  int opt;
  int (*pfunc)(void **args, MEM *mem);
};

static int help(void **args, MEM *mem);
static int process(void **args, MEM *mem);
static int shm(void **args, MEM *mem);
static int motor(void **args, MEM *mem);
static int servo(void **args, MEM *mem);
static int queue(void **args, MEM *mem);


static char *isRunning(int pid)
{
  static char str[15] = {0};

  memset(str, 0, sizeof(str));

  if(pid < 0)
  {
    strncpy(str, "not running.", strlen("not running"));
  }else
  {
    strncpy(str, "running.", strlen("running"));
  }
  
  return str;
}

int main(int argc, char *argv[])
{
  int ret;
  MEM *mem = NULL;
  int opt = -1;
  int opt_index;
  const char *const soptions = "h";

  const struct option options[] = 
  {
    {"help"   , ARGS_NO, NULL, OPT_HELP},
    {"process", ARGS_NO, NULL, OPT_PROCESS},
    {"shm"    , ARGS_NO, NULL, OPT_SHM},
    {"motor"  , ARGS_NO, NULL, OPT_MOTOR},
    {"servo"  , ARGS_NO, NULL, OPT_SERVO},
    {"queue"  , ARGS_NO, NULL, OPT_QUEUE},
    {NULL  , ARGS_NO, NULL, 0}
  };

  const struct table tab[] = 
  {
    {.opt = OPT_HELP    , .pfunc = help},
    {.opt = OPT_PROCESS , .pfunc = process},
    {.opt = OPT_SHM     , .pfunc = shm},
    {.opt = OPT_MOTOR   , .pfunc = motor},
    {.opt = OPT_SERVO   , .pfunc = servo},
    {.opt = OPT_QUEUE   , .pfunc = queue}
  };

  const int table_size = sizeof(tab)/sizeof(tab[0]);

  // memset(&mem, 0, sizeof(mem));

  while((opt = getopt_long(argc, argv, soptions, options, &opt_index)) != -1)
  {
    switch(opt)
    {
      case OPT_HELP:
      case OPT_PROCESS:
      case OPT_SHM:
      case OPT_MOTOR:
      case OPT_SERVO:
      case OPT_QUEUE:
        break;
    }
  }

  // ret = mem_init(&mem);
  mem = mem_get();
  if(mem == NULL)
  {
    return 1;
  }
  
  ret = -1;

  for(int i = 0; i < table_size; i++)
  {
    if(opt_index == tab[i].opt)
    {
      ret = tab[i].pfunc(argv, mem);
      break;
    }
  }

  return ret;
}

static int help(void **args, MEM *mem)
{
  const char *program = args[0];
  (void)mem;

  fprintf(stderr, "Usage: %s <option>\n", program);
  fprintf(stderr, "options:\n");
  fprintf(stderr, "--help    \t\t Show how to use commands.\n");
  fprintf(stderr, "--process \t\t Show all processes is running\n");
  fprintf(stderr, "--shm     \t\t Show id shared memory used by program\n");
  fprintf(stderr, "--motor   \t\t Show motor status\n");
  fprintf(stderr, "--servo   \t\t Show servo status\n");
  fprintf(stderr, "--queue   \t\t Show Queue used by program\n");
  return 0;
}

static int process(void **args, MEM *mem)
{
  (void)args;

  for(int i = 0; i < PROCESS_AMOUNT; i++)
  {
    process_t *proc = &mem->procs[i];
    printf("process[%d]: %s %s\n", proc->pid, proc->name, isRunning(proc->pid));
  }

  return 0;
}

static int shm(void **args, MEM *mem)
{
  (void)args;

  FILE *pipe;
  char command[256] = {0};
  char command_out[256] = {0};
  printf("SHM_PROCESSES_KEY\n");
  snprintf(command,sizeof(command), "ipcs -m | grep %08X", SHM_PROCESSES_KEY);

  pipe = popen(command, "r");
  if(pipe == NULL)
    return -1;

  while(fgets(command_out, sizeof(command_out), pipe) != NULL)
    printf("%s\n", command_out);

  pclose(pipe);

  return 0;
}

static int motor(void **args, MEM *mem)
{
  (void)args;

  generic_st *m = &mem->motor;

  printf("Motor Status:\n");
  printf("Motor protocol id: %d\n", m->id);
  printf("Motor status: %d\n", m->status);
  printf("Motor current command: %s\n", m->command);

  return 0;
}

static int servo(void **args, MEM *mem)
{
  (void)args;

  generic_st *s = &mem->servo;

  printf("Servo Status:\n");
  printf("Servo protocol id: %d\n", s->id);
  printf("Servo status: %d\n", s->status);
  printf("Servo current command: %s\n", s->command);

  return 0;
}

static int queue(void **args, MEM *mem)
{
  (void)args;

  FILE *pipe;
  char command[256] = {0};
  char command_out[256] = {0};
  printf("QUEUE_MANAGER_ID\n");
  snprintf(command,sizeof(command), "ipcs -q | grep %08X", QUEUE_MANAGER_ID);

  pipe = popen(command, "r");
  if(pipe == NULL)
    return -1;

  while(fgets(command_out, sizeof(command_out), pipe) != NULL)
    printf("%s\n", command_out);

  pclose(pipe);

  return 0;
}
