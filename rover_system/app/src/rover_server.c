#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <queue/queue.h>
#include <log/log.h>
#include <app.h>
#include <protocol.h>
#include <signal/signal.h>
#include <signal.h>

#define MAX 4096
#define PORT 8080
#define SA struct sockaddr

#define ROVER_SERVER "ROVER_SERVER"

#define RUNNING 1
#define STOPPED 0

static int server_running = RUNNING;

typedef enum COMM_ERROR
{
  ERROR_RECV = -1,
  ERROR_CLIENT_DISCONNECTED = 0
} COMM_ERROR;

static int queue_id = -1;


void server_state_change(int s);
void end_server(int s);

// Function designed for chat between client and server.
void func(int sockfd)
{
  queue_st queue;
  char buff[MAX];
  int ret;
  // infinite loop for chat

  while (server_running)
  {
    bzero(buff, MAX);

    // read the message from client and copy it in buffer
    ret = recv(sockfd, buff, sizeof(buff), 0);

    switch (ret)
    {

    case ERROR_RECV:
      log(LOG_INFO, ROVER_SERVER, "Error in recv");
      close(sockfd);
      return;

    case ERROR_CLIENT_DISCONNECTED:
      log(LOG_INFO, ROVER_SERVER, "Client disconnected");
      close(sockfd);
      return;

    default:
      log(LOG_INFO, ROVER_SERVER, buff);
    }

    memcpy(queue.bData, buff, sizeof(protocol_t));
    queue.queueType = 1;
    ret = queue_send(queue_id, &queue, sizeof(protocol_t));
    if (ret < 0)
    {
      log(LOG_INFO, ROVER_SERVER, "Error Queue Send.");
    }
  }
  close(sockfd);
}

// Driver function
int main()
{
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  signal_register(server_state_change, SIGINT);
  signal_register(end_server, SIGTERM);

  queue_id = queue_init(QUEUE_MANAGER_ID);
  if (queue_id < 0)
  {
    log(LOG_INFO, ROVER_SERVER, "Queue init failed");
    exit(1);
  }
  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    log(LOG_INFO, ROVER_SERVER, "Socket creation failed.");
    exit(1);
  }
  else
  {
    log(LOG_INFO, ROVER_SERVER, "Socket successfully created");
  }
  
  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  // Binding newly created socket to given IP and verification
  if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
  {
    log(LOG_INFO, ROVER_SERVER, "Bind failed.");
    exit(1);
  }
  else
  {
    log(LOG_INFO, ROVER_SERVER, "Server successfully binded.");
  }

  while (server_running)
  {
    // Now server is ready to listen and verification
    if ((listen(sockfd, 1)) != 0)
    {
      log(LOG_INFO, ROVER_SERVER, "Listen failed.");
      exit(1);
    }
    else
    {
      log(LOG_INFO, ROVER_SERVER, "Server listening.");
    }
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
      log(LOG_INFO, ROVER_SERVER, "Server accept failed.");
      exit(1);
    }
    else
    {
      log(LOG_INFO, ROVER_SERVER, "Server accept client.");
    }

    // Function for chatting between client and server
    func(connfd);
  }
  // After chatting close the socket
  close(sockfd);
}

void server_state_change(int s)
{
  server_running = 0;
}

void end_server(int s)
{
  log(LOG_INFO, ROVER_SERVER, "Server unlaunched.");
  exit(s);
}
