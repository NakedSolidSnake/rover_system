#ifndef __SIGNAL_H
#define __SIGNAL_H

int signal_register(void(*callback)(int sig), int sig);
int notify_process(int pid, int sig);

#endif
