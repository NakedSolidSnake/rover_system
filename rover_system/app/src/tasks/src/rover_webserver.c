#include <includes.h>
#include <log/log.h>
#include <server/server_tcp.h>
#include <sharedmemory/sharedmemory.h>
#include <app.h>
#include <signal/signal.h>
#include <rover_webserver.h>
#include <context.h>

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

static Context_st webserver_context;

static int sender(char *buffer, int *size);
static int receive(char *buffer, int size);
static int sendPage(char *buffer, int *size);

static void init(void);
static void update_time(int s);
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
          .timeout = 0,
          .cb.recv = receive,
          .cb.send = sender
  };

  init();

  Server_init(&server);
  while(1)
  {
    Server_exec(&server);
    if(webserver_context.states.update_time){

      webserver_context.states.update_time = 0;
    }
  }
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
            "<p>Motors: [ direction : %d ] [Power : %d]</p>"
            "<p>Servo: [Position : %d]</p>"
            "<p>Ultrasound: [Distance : %f]</p>"
            "<p>LCD: [Line 1 : %s] [Line 2: %s]</p>"
            "</h2>"
        "</main>"
    "</body>"
"</html>";

  snprintf(b, sizeof(b), msg, webserver_context.mem->status.motor_status.direction,
                              webserver_context.mem->status.motor_status.power,
                              webserver_context.mem->status.servo_status.position,
                              webserver_context.mem->status.ultrasound_status.distance,
                              webserver_context.mem->status.lcd16_status.msg_line1,
                              webserver_context.mem->status.lcd16_status.msg_line2);

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

static void init(void)
{
  memset(&webserver_context, 0, sizeof(Context_st));

  webserver_context.mem = mem_get();
  if(!webserver_context.mem)
  {
    logger(LOGGER_INFO, ROVER_WEBSERVER, "Memory not initialized");
    exit(EXIT_FAILURE);
  }

  signal_register(update_time, SIGUPDATETIME);
  signal_register(end_server, SIGTERM);

  logger(LOGGER_INFO, ROVER_WEBSERVER, "WEBSERVER initialized");

}

static void update_time(int s)
{
  webserver_context.states.update_time = 1;
}

static void end_server(int s)
{
  logger(LOGGER_INFO, ROVER_WEBSERVER, "Server unlaunched.");
  exit(s);
}