#include <signal/signal.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static int state = 0;
static int end = 0;

void handler(int s)
{
  state = 1;
}

void alarmed(int s)
{
  end = 1;
}

int main()
{
  signal_register(handler, SIGINT);
  signal(SIGALRM, alarmed);

  alarm(5);


  while(1){
    if(state)
      break;
    if(end)
      break;
  }

  if(state){
    printf("Received signal.\n");
    return 0;
  }

  printf("Alarmed ringed.\n");
  return -1;
}
