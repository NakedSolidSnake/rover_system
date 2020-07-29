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
    .cb.recv = receive,
    .cb.send = sender
  };

  init();

  Server_init(&server);
  Server_exec(&server);

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

  server_context.sema.id = -1;
  server_context.sema.sema_count = 1;
  server_context.sema.state = LOCKED;
  server_context.sema.master = SLAVE;

  semaphore_init(&server_context.sema, SEMA_ID);
  
  signal_register(update_time, SIGUPDATETIME);
  signal_register(end_server, SIGTERM);

  logger(LOGGER_INFO, ROVER_SERVER, "SERVER initialized");
}

static void update_time(int s)
{
  server_context.states.update_time = 1;
}

void end_server(int s)
{
  logger(LOGGER_INFO, ROVER_SERVER, "Server unlaunched.");
  exit(s);
}