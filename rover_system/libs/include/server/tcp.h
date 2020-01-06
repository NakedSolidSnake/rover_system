#ifndef __TCP_H
#define __TCP_H

#define MAX_PORT_LEN 6
#define CONNECTIONS  5
#define BSIZE        1024

typedef struct server
{
  int sock;
  char port[MAX_PORT_LEN];
}server_t;

int server_setPort(server_t *server, const char *port);
int server_init(server_t *server);
int server_exec(const server_t *server);

#endif
