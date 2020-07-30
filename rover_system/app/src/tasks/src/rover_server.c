#include <includes.h>
#include <queue/queue.h>
#include <log/log.h>
#include <server/server_tcp.h>
#include <protocol.h>
#include <signal/signal.h>
#include <rover_server.h>
#include <context.h>

#define ROVER_SERVER "ROVER_SERVER"

static int sender(char *buffer, int *size);
static int receive(char *buffer, int size);

static Context_st server_context;

static void init(void);
static void update_time(int s);
static void end_server(int s);



#ifdef PROCESS
int main(int argc, char const *argv[])
{
    (void)rover_server(NULL);
    return 0;
}
#endif

void *rover_server(void *args)
{
  (void)args;
  Server_t server =
  {
    .socket = -1,
    .port = "8080",
    .timeout = 250000,
    .cb.recv = receive,
    .cb.send = sender
  };

  init();

  Server_init(&server);

  while(1)
  {
    Server_exec(&server);
    if(server_context.states.update_time){
      int index = get_pid(getpid());
      if(index >= 0 &&  index < server_context.mem->process_amount)
      {
        clock_gettime(CLOCK_MONOTONIC, &server_context.current);
        // if (semaphore_lock(&server_context.sema_update) == 0)
        {
          server_context.mem->processes[index].update = (time_t)((double)server_context.current.tv_sec + (double)server_context.current.tv_nsec/(double)1000000000);
          // semaphore_unlock(&server_context.sema_update);
        }
      }
      server_context.states.update_time = 0;
      alarm(PROCESS_CICLE_SECONDS);
    }
  }
  
  Server_close(&server);

  return NULL;
}



static int sender(char *buffer, int *size)
{
  memset(server_context.queue.bData, 0, sizeof(server_context.queue.bData));

  if (queue_recv(server_context.mem->queue_server_id, &server_context.queue, sizeof(server_context.queue.bData), 150000000) < 0)
  {
    *size = 0;
    return 0;
  }
  
  loggerArgs(LOGGER_INFO, ROVER_SERVER, "Server send: %s", server_context.queue.bData);

  memcpy(buffer, server_context.queue.bData, MAX_BUFFER_SEND_RECV);  
  *size = strlen(buffer);
  return 0;
}

static int receive(char *buffer, int size)
{
  int ret = -1;
  memset(server_context.queue.bData, 0, sizeof(server_context.queue.bData));
  memcpy(server_context.queue.bData, buffer, sizeof(protocol_t));
  server_context.queue.queueType = 1;
  ret = queue_send(server_context.mem->queueid, &server_context.queue, sizeof(protocol_t));
  if (ret < 0)
    logger(LOGGER_INFO, ROVER_SERVER, "Error Queue Send.");

  memset(buffer, 0, MAX_BUFFER_SEND_RECV);

  return 0;
}


static void init(void)
{
  memset(&server_context, 0, sizeof(Context_st));

  server_context.mem = mem_get();
  if(!server_context.mem)
  {
    logger(LOGGER_INFO, ROVER_SERVER, "Memory not initialized");
    exit(EXIT_FAILURE);
  }

  server_context.sema_update.id = -1;
  server_context.sema_update.sema_count = 1;
  server_context.sema_update.state = LOCKED;
  server_context.sema_update.master = SLAVE;

  semaphore_init(&server_context.sema_update, SEMA_UPDATE_ID);
  
  signal_register(update_time, SIGUPDATETIME);
  signal_register(update_time, SIGALRM);
  signal_register(end_server, SIGTERM);

  logger(LOGGER_INFO, ROVER_SERVER, "SERVER initialized");
  alarm(PROCESS_CICLE_SECONDS);
}

static void update_time(int s)
{
  server_context.states.update_time = 1;
}

void end_server(int s)
{
  logger(LOGGER_INFO, ROVER_SERVER, "Server unlaunched.");
  semaphore_unlock(&server_context.sema_update);
  semaphore_unlock(&server_context.sema_message);
  exit(s);
}