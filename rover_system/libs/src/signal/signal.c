#include <signal/signal.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int signal_register(void(*callback)(int sig), int sig)
{
  struct sigaction s;
  int ret;

  if(!callback)
    return -1;

  s.sa_handler = callback;

  ret = sigfillset(&s.sa_mask);
  if(ret < 0){
    return -1;
  }

  s.sa_flags = 0;

  ret = sigaction(sig, &s, 0); 
  if(ret < 0){
    return -1;
  }

  return 0;
}


int notify_process(int pid, int sig)
{
  return kill((pid_t) pid, sig);
}
