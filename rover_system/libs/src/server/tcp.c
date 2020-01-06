#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <server/tcp.h>

static int acceptClientConnection(int serverSock);
static void handleClientConnection(int clientSock);

static void *run(void *args);

int server_setPort(server_t *server, const char *port)
{
  if(!server)
    return -1;

  strncpy(server->port, port, MAX_PORT_LEN);
  return 0;
}

int server_init(server_t *server)
{
  if(!server)
    return -1;

  struct addrinfo hints, *servinfo, *p;

  int rv;
  int yes = 1;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;


  if((rv = getaddrinfo(NULL, server->port, &hints, &servinfo)) != 0)
  {
    //log
    return 1;
  }

  for(p = servinfo; p != NULL; p = p->ai_next)
  {
    if((server->sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
    {
      //log error
      continue;
    }

    if(setsockopt(server->sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
      //log error 
      return 1;
    }

    if(bind(server->sock, p->ai_addr, p->ai_addrlen) == -1)
    {
      close(server->sock);
      //log error 
      continue;
    }

    break;
  }

  freeaddrinfo(servinfo);

  if(p == NULL)  
  {
    //log error 
    return 1;
  }

  if(listen(server->sock, CONNECTIONS) == -1)
  {
    //log error 
    return 1;
  }

  return 0;
}

int server_exec(const server_t *server)
{
  int fdmax;
  fd_set master;
  fd_set read_fds;
  int ret;
  int newClient;

  int i;

  FD_ZERO(&master);
  FD_ZERO(&read_fds);

  FD_SET(server->sock, &master);
  fdmax = server->sock;

  for(;;)
  {
    read_fds = master;
    
    ret = select(fdmax + 1, &read_fds, NULL, NULL, NULL);
    if(ret == -1)
    {
      //log error 
      return 1;
    }else if(ret == 0)
    {
      //timeout
    }

    for( i = 0; i <= fdmax; i++)
    {
      if(FD_ISSET(i, &read_fds))
      {
        newClient = acceptClientConnection(i); 
        //launch a thread or fork
        pthread_t thread;
        pthread_create(&thread, NULL, run, (void *)&newClient);
        //handleClientConnection(newClient);
      }
    }
  }

  close(server->sock);
  return 0;

}

static int acceptClientConnection(int serverSock)
{
  int clientSock;
  struct sockaddr_in clientAddr;
  unsigned int clientLen;

  clientLen = sizeof(clientAddr);

  clientSock = accept(serverSock, (struct sockaddr *)&clientAddr, &clientLen);
  if(clientSock < 0)
  {
    //log error 
    return -1;
  }

  return clientSock;
}

static void handleClientConnection(int clientSock)
{
  char b[BSIZE];
  int recvSize;

  while(1)
  {
    recvSize = recv(clientSock, b, BSIZE, 0);
    if(recvSize < 0)
    {
      //log error 
      close(clientSock);
      return ;
    }
    if(recvSize == 0)
    {
      //log end connection 
      close(clientSock);
      return ;
    }else 
    {
      //callback register
      send(clientSock, b, strlen(b), 0);
    }
  }
}


static void *run(void *args)
{
  int clientSock = *((int *)args);
  handleClientConnection(clientSock);

  return NULL;
}
