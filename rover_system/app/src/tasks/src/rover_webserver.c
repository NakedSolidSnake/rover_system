#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <log/log.h>
#include <server/server_tcp.h>
#include <sharedmemory/sharedmemory.h>
#include <app.h>
#include <signal/signal.h>
#include <signal.h>
#include <unistd.h>
#include <rover_webserver.h>

#define ROVER_WEBSERVER "ROVER_WEBSERVER"

#define MAX_LEN 10
#define CLEAR 0
#define SET 1

typedef int (*cb)(char *buffer, int *size);

typedef struct
{
  char method[MAX_LEN];
  int size;
  cb call_method;
  int set;
} method_t;

static int sender(char *buffer, int *size);
static int receive(char *buffer, int size);
static int sendPage(char *buffer, int *size);
static void end_server(int s);

static method_t methods[] =
    {
        {.method = "GET", .size = 3, .call_method = sendPage, .set = 0},
};

#ifdef PROCESS
int main(int argc, char const *argv[])
{
    (void)rover_webserver(NULL);
    return 0;
}
#endif

void *rover_webserver(void *args)
{
  (void)args;
  Server_t server =
      {
          .socket = -1,
          .port = "8081",
          .cb.recv = receive,
          .cb.send = sender};

  signal_register(end_server, SIGTERM);

  Server_init(&server);
  Server_exec(&server);
}

static void end_server(int s)
{
  logger(LOGGER_INFO, ROVER_WEBSERVER, "Server unlaunched.");
  exit(s);
}

static int sender(char *buffer, int *size)
{
  for (int i = 0; i < sizeof(methods) / sizeof(methods[0]); i++)
  {
    if (methods[i].set == SET)
    {
      methods[i].set = CLEAR;
      methods[i].call_method(buffer, size);
      return 0;
    }
  }

  return 1;
}

static int receive(char *buffer, int size)
{
  printf("%*s\n", 6, buffer);

  for (int i = 0; i < sizeof(methods) / sizeof(methods[0]); i++)
  {
    if (!strncmp(methods[i].method, buffer, methods[i].size))
    {
      methods[i].set = SET;
      return 0;
    }
  }
  //send error
  return 1;
}

static int sendPage(char *buffer, int *size)
{

  MEM *mem = NULL;

  mem = mem_get();
  if(mem == NULL)
  {
    return 1;
  }

  generic_st *m = &mem->motor;
  generic_st *s = &mem->servo;


  char b[512];

  char msg[] =
  "<html>"
   " <header>"

    "</header>"
    "<body>"
        "<title>"
            "Rover Webserver"
        "</title>"

        "<main>"
            "<p>Motors: status %s</p>"
            "<p>Servo: status %s</p>"
            "</h2>"
        "</main>"
    "</body>"
"</html>";

  snprintf(b, sizeof(b), msg, m->command, s->command);

  memset(buffer, 0, MAX_BUFFER_SEND_RECV);
  
  sprintf(buffer, "HTTP/1.1 200 OK\r\n"
                  "Connection: close\r\n"
                  "Content-Length: %ld\r\n"
                  "Content-Type: %s\r\n"
                  "\r\n"
                  "%s",
                   strlen(b), "text/html", b);
  *size = strlen(buffer);
  return 0;
}
