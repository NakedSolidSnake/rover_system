#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sharedmemory/sharedmemory.h>
#include <app.h>

#define ARGS_NO 0
#define ARGS_REQ 1
#define ARGS_OPT 2

typedef struct commands
{
  char command[30];
  int id;
}commands_st;

typedef enum opt_e
{
  OPT_HELP = 0,
  OPT_PROCESS,
  OPT_SHM,
  OPT_MOTOR,
  OPT_SERVO,
  OPT_QUEUE,
  OPT_ULTRASOUND,
  OPT_LCD,
  OPT_SEMA,
  OPT_IPCS, 
  OPT_MONITOR_ON,
  OPT_MONITOR_OFF
} opt_et;

struct table
{
  int opt;
  int (*pfunc)(char **args, MEM *mem);
};

static int help(char **args, MEM *mem);
static int process(char **args, MEM *mem);
static int shm(char **args, MEM *mem);
static int motor(char **args, MEM *mem);
static int servo(char **args, MEM *mem);
static int queue(char **args, MEM *mem);
static int ultrasound(char **args, MEM *mem);
static int lcd16(char **args, MEM *mem);
static int sema(char **args, MEM *mem);
static int ipcs(char **args, MEM *mem);
static int monitor_on(char **args, MEM *mem);
static int monitor_off(char **args, MEM *mem);

static char *isRunning(int pid)
{
  static char str[15] = {0};

  memset(str, 0, sizeof(str));

  if (pid < 0)
  {
    strncpy(str, "not running.", strlen("not running"));
  }
  else
  {
    strncpy(str, "running.", strlen("running"));
  }

  return str;
}

int main(int argc, char **argv)
{
  int ret;
  MEM *mem = NULL;
  int opt = -1;
  int opt_index;
  const char *const soptions = "h";

  const struct option options[] =
      {
          {"help", ARGS_NO, NULL, OPT_HELP},
          {"process", ARGS_NO, NULL, OPT_PROCESS},
          {"shm", ARGS_NO, NULL, OPT_SHM},
          {"motor", ARGS_NO, NULL, OPT_MOTOR},
          {"servo", ARGS_NO, NULL, OPT_SERVO},
          {"queue", ARGS_NO, NULL, OPT_QUEUE},
          {"ultrasound", ARGS_NO, NULL, OPT_ULTRASOUND},
          {"lcd", ARGS_NO, NULL, OPT_LCD},
          {"sema", ARGS_NO, NULL, OPT_SEMA},
          {"ipcs", ARGS_NO, NULL, OPT_IPCS},
          {"monitor_on", ARGS_NO, NULL, OPT_MONITOR_ON},
          {"monitor_off", ARGS_NO, NULL, OPT_MONITOR_OFF},
          {NULL, ARGS_NO, NULL, 0}};

  const struct table tab[] =
      {
          {.opt = OPT_HELP, .pfunc = help},
          {.opt = OPT_PROCESS, .pfunc = process},
          {.opt = OPT_SHM, .pfunc = shm},
          {.opt = OPT_MOTOR, .pfunc = motor},
          {.opt = OPT_SERVO, .pfunc = servo},
          {.opt = OPT_QUEUE, .pfunc = queue},
          {.opt = OPT_ULTRASOUND, .pfunc = ultrasound},
          {.opt = OPT_LCD, .pfunc = lcd16},
          {.opt = OPT_SEMA, .pfunc = sema},
          {.opt = OPT_IPCS, .pfunc = ipcs},
          {.opt = OPT_MONITOR_ON, .pfunc = monitor_on},
          {.opt = OPT_MONITOR_OFF, .pfunc = monitor_off},
      };

  const int table_size = sizeof(tab) / sizeof(tab[0]);

  while ((opt = getopt_long(argc, argv, soptions, options, &opt_index)) != -1)
  {
    switch (opt)
    {
    case OPT_HELP:
    case OPT_PROCESS:
    case OPT_SHM:
    case OPT_MOTOR:
    case OPT_SERVO:
    case OPT_QUEUE:
    case OPT_ULTRASOUND:
    case OPT_LCD:
    case OPT_IPCS:
    case OPT_MONITOR_ON:
    case OPT_MONITOR_OFF:
      break;

    default: 
      help(argv, NULL);
      return -1;
    }
  }

  mem = mem_get();
  if (mem == NULL)
  {
    return 1;
  }

  ret = -1;

  for (int i = 0; i < table_size; i++)
  {
    if (opt_index == tab[i].opt)
    {
      ret = tab[i].pfunc(argv, mem);
      break;
    }
  }

  return ret;
}

static int help(char **args, MEM *mem)
{
  const char *program = args[0];
  (void)mem;

  fprintf(stderr, "Usage: %s <option>\n", program);
  fprintf(stderr, "options:\n");
  fprintf(stderr, "--help    \t\t Show how to use commands.\n");
  fprintf(stderr, "--process \t\t Show all processes is running\n");
  fprintf(stderr, "--shm     \t\t Show id shared memory used by program\n");
  fprintf(stderr, "--sema     \t\t Show id semaphore used by program\n");
  fprintf(stderr, "--motor   \t\t Show motor status\n");
  fprintf(stderr, "--servo   \t\t Show servo status\n");
  fprintf(stderr, "--ultrasound   \t\t Show ultrasound status\n");
  fprintf(stderr, "--lcd   \t\t Show lcd status\n");
  fprintf(stderr, "--queue   \t\t Show Queue used by program\n");
  fprintf(stderr, "--ipcs   \t\t Show all IPC used by program\n");
  fprintf(stderr, "--monitor_on   \t\t Sets Monitor in running state\n");
  fprintf(stderr, "--monitor_off   \t\t Sets Monitor in pause state\n");
  return 0;
}

static int process(char **args, MEM *mem)
{
  (void)args;

  for (int i = 0; i < PROCESS_AMOUNT; i++)
  {
    process_st *process = &mem->processes[i];
    printf("process[%d]: %s %s\n", process->pid, process->name, isRunning(process->pid));
  }

  return 0;
}

static int shm(char **args, MEM *mem)
{
  (void)args;

  FILE *pipe;
  char command[256] = {0};
  char command_out[256] = {0};
  printf("SHM_PROCESSES_KEY\n");
  snprintf(command, sizeof(command), "ipcs -m | grep %08x", SHM_PROCESSES_KEY);

  pipe = popen(command, "r");
  if (pipe == NULL)
    return -1;

  while (fgets(command_out, sizeof(command_out), pipe) != NULL)
    printf("%s", command_out);

  pclose(pipe);

  return 0;
}

static int motor(char **args, MEM *mem)
{
  (void)args;

  motor_status_st *m_status = &mem->status.motor_status;

  printf("Motor Status:\n");
  printf("Motor power: %d.\n", m_status->power);
  printf("Motor direction: %d.\n", m_status->direction);

  return 0;
}

static int servo(char **args, MEM *mem)
{
  (void)args;

  servo_status_st *s_status = &mem->status.servo_status;

  printf("Servo Status:\n");
  printf("Servo position: %d.\n", s_status->position);

  return 0;
}

static int queue(char **args, MEM *mem)
{
  (void)args;

  commands_st commands[] = 
  {
    {.command = "ipcs -q | grep %08x", .id = QUEUE_MANAGER_ID},
    {.command = "ipcs -q | grep %08x", .id = QUEUE_SERVER_ID},
  };

  int commands_amount = sizeof(commands)/sizeof(commands[0]);

  FILE *pipe;
  char command[256];
  char command_out[256];
  printf("QUEUE_ID's\n");
  for (int i = 0; i < commands_amount; i++)
  {
    memset(command, 0, sizeof(command));
    memset(command_out, 0, sizeof(command_out));
    snprintf(command, sizeof(command), commands[i].command, commands[i].id);

    pipe = popen(command, "r");
    if (pipe == NULL)
      return -1;

    while (fgets(command_out, sizeof(command_out), pipe) != NULL)
      printf("%s", command_out);

    pclose(pipe);
  }
  return 0;
}

static int ultrasound(char **args, MEM *mem)
{
  (void)args;

  ultrasound_status_st *ultra_status = &mem->status.ultrasound_status;

  printf("Ultrasound Status:\n");
  printf("Ultrasound distance: %f.\n", ultra_status->distance);
 
  return 0;
}

static int lcd16(char **args, MEM *mem)
{
  (void)args;

  lcd16_status_st *lcd_status = &mem->status.lcd16_status;

  printf("LCD Status:\n");
  printf("LCD message line 1: %s.\n", lcd_status->msg_line1);
  printf("LCD message line 2: %s.\n", lcd_status->msg_line2);
 
  return 0;
}

static int sema(char **args, MEM *mem)
{
  (void)args;

  FILE *pipe;
  char command[256] = {0};
  char command_out[256] = {0};
  printf("SEMAPHORE_ID\n");
  snprintf(command, sizeof(command), "ipcs -s | grep %08x", SEMA_MESSAGE_ID);

  pipe = popen(command, "r");
  if (pipe == NULL)
    return -1;

  while (fgets(command_out, sizeof(command_out), pipe) != NULL)
    printf("%s", command_out);

  pclose(pipe);

  return 0;
}

static int ipcs(char **args, MEM *mem)
{
  queue(args, mem);
  shm(args, mem);
  sema(args, mem);
  printf("Queue Server ID in memory: %d.\n", mem->queue_server_id);
  printf("Queue Manager ID in memory: %d.\n", mem->queueid);
  printf("Semaphore Message ID in memory: %d.\n", mem->sema_message.id);
  printf("Semaphore Update ID in memory: %d.\n", mem->sema_update.id);
  printf("Shared Memory ID in memory: %d.\n", mem->shm.id);
  printf("Shared Memory ADDRESS in memory: %p.\n", mem->shm.shm);
  return 0;
}

static int monitor_on(char **args, MEM *mem)
{  
  mem->monitor_enable = 1;
  printf("monitor is running.\n");
  return 0;
}

static int monitor_off(char **args, MEM *mem)
{
  mem->monitor_enable = 0;
  printf("monitor is stopped.\n");
  return 0;
}
