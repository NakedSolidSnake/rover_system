#include <signal/signal.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    fprintf(stderr, "Usage: %s <pid_process> <signal>\n", argv[0]);
    return 1;
  }

  int pid = atoi(argv[1]);
  int sig = atoi(argv[2]);

  notify_process((pid_t)pid, sig);
  return 0;
}
