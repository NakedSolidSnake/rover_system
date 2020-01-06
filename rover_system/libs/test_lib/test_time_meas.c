#include <util/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  TimeMeas_t *t = createTimeMeas();
  if(!t)
    return EXIT_FAILURE;

  getStartTime(t);
  sleep(1);
  getFinishTime(t);

  long long resul = getDiffMeas(t);

  if(resul < 1000)
    return EXIT_FAILURE;

  printf("time diff %ld\n", resul);

  destroyTimeMeas(t);

  return EXIT_SUCCESS;
}

