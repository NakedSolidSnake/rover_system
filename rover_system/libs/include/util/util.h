#ifndef __UTIL_H
#define __UTIL_H

typedef struct time_measure TimeMeas_t;

TimeMeas_t *createTimeMeas(void);
int getStartTime(TimeMeas_t *t);
int getFinishTime(TimeMeas_t *t);
long long getDiffMeas(TimeMeas_t *t);
int destroyTimeMeas(TimeMeas_t *t);

#endif
