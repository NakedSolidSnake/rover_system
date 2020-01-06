#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <util/util.h>


struct time_measure
{
  long long timeStart;
  long long timeEnd;
};


static double getCurrentTime(void);

TimeMeas_t *createTimeMeas(void)
{
  TimeMeas_t *t = (TimeMeas_t *)malloc(sizeof(TimeMeas_t));
  if(!t)
    return NULL;

  memset(t, 0, sizeof(TimeMeas_t));
  return t;
}

int getStartTime(TimeMeas_t *t)
{
  if(!t)
    return -1;

  t->timeStart = getCurrentTime();

  return 0;  
}

int getFinishTime(TimeMeas_t *t)
{
  if(!t)
    return -1;

  t->timeEnd = getCurrentTime();

  return 0;
}

long long getDiffMeas(TimeMeas_t *t)
{
  if(!t)
    return -1;

  return t->timeEnd - t->timeStart;
}

int destroyTimeMeas(TimeMeas_t *t)
{
  if(!t)
    return -1;

  free(t);

  return 0;
}

static double getCurrentTime(void)
{
  struct timeval tv;
  double time_us;
  gettimeofday(&tv, NULL);

  time_us = tv.tv_sec * 1000000 + tv.tv_usec;
  return time_us;
}
