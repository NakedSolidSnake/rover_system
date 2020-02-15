#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <queue/queue.h>
#include <log/log.h>
#include <server/server_tcp.h>
#include <app.h>
#include <protocol.h>
#include <signal/signal.h>
#include <signal.h>
#include <unistd.h>

#define ROVER_SERVER "ROVER_SERVER"

static int sender(char *buffer, int *size);
static int receive(char *buffer, int size);

void end_server(int s);

static int queue_id = -1;
static queue_st queue;

// Driver function
int main()
{
  Server_t server = 
  {
    .socket = -1,
    .port = "8080",
    .cb.recv = receive,
    .cb.send = sender
  };

  signal_register(end_server, SIGTERM);

  queue_id = queue_init(QUEUE_MANAGER_ID);
  if (queue_id < 0)
  {
    logger(LOGGER_INFO, ROVER_SERVER, "Queue init failed");
    exit(1);
  }

  Server_init(&server);
  Server_exec(&server);  
}

void end_server(int s)
{
  logger(LOGGER_INFO, ROVER_SERVER, "Server unlaunched.");
  exit(s);
}

static int sender(char *buffer, int *size)
{    
    char message[] = "OK";
    *size = strlen(message);
    strncpy(buffer, message, *size);
    return 0;
}

static int receive(char *buffer, int size)
{
    int ret = -1;    
    memcpy(queue.bData, buffer, sizeof(protocol_t));
    queue.queueType = 1;
    ret = queue_send(queue_id, &queue, sizeof(protocol_t));
    if (ret < 0)    
      logger(LOGGER_INFO, ROVER_SERVER, "Error Queue Send.");

    memset(buffer, 0, MAX_BUFFER_SEND_RECV);
    
    return 0;
}
