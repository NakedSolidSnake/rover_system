#include <includes.h>
#include <rover_monitor.h>
#include <app.h>
#include <log/log.h>

#define ROVER_MONITOR  "ROVER_MONITOR"

static int verifyTime( time_t timeout, process_st *process);

int main(int argc, char const *argv[])
{
    struct timespec  current;
    sema_t sema_update;
    time_t t = 1000000000;
    int i;
    int ret;

    MEM *mem = mem_get();
    if(mem == NULL)
        return -1;

    sema_update.id = -1;
    sema_update.sema_count = 1;
    sema_update.state = LOCKED;
    sema_update.master = SLAVE;

    semaphore_init(&sema_update, SEMA_UPDATE_ID);

    clock_gettime(CLOCK_MONOTONIC, &current);

    // if (semaphore_lock(&sema_update) == 0)
    {
        for (i = 0; i < (mem->process_amount - 1); i++)
        {
            process_st *process = &mem->processes[i];
            process->old = process->update = (time_t)((double)current.tv_sec + (double)current.tv_nsec / (double)1000000000);
        }
        // semaphore_unlock(&sema_update);
    }

    while(1)
    {
        sleep(MONITOR_CICLE_SECONDS);

        if(!mem->monitor_enable)
            continue;        

        for(i = 0; i < (mem->process_amount - 1); i++)
        {   
            process_st *process = &mem->processes[i];       
            ret = verifyTime(t, process);

            if (!ret)
            {
                // if (semaphore_lock(&sema_update) == 0)
                {
                    process->old = process->update;
                    process->miss_count = 0;
                    // semaphore_unlock(&sema_update);
                }
            }
            else
            {

                if(process->miss_count == MONITOR_MAX_TRY){
                    loggerArgs(LOGGER_INFO, ROVER_MONITOR, "Process %s unable to respond", process->name);
                    loggerArgs(LOGGER_INFO, ROVER_MONITOR, "Killing process %s", process->name);
                    kill(process->pid, SIGKILL);
                    
                    loggerArgs(LOGGER_INFO, ROVER_MONITOR, "Recover process %s", process->name);
                    pid_t pid = fork();
                    if (pid == 0)
                    {
                        execl(process->name, process->name, NULL);
                    }
                    else if (pid > 0)
                    { 
                        usleep(10000);
                        signal(SIGCHLD,SIG_IGN); //ignores child signal   
                        // if (semaphore_lock(&sema_update) == 0)
                        {
                            process->pid = pid;                        
                            memoryWrite(mem, (void *)process, i);
                            process->miss_count = 0; 
                            // semaphore_unlock(&sema_update);  
                        }                                             
                    }
                }else {
                    loggerArgs(LOGGER_INFO, ROVER_MONITOR, "send signal to process %s pid %d\n", process->name, process->pid);
                    //wait for response
                    kill(process->pid, SIGUPDATETIME);
                    // if (semaphore_lock(&sema_update) == 0)
                    {
                        process->miss_count ++;
                        // semaphore_unlock(&sema_update);
                    }
                    
                }
            }
        }        
    }    
    return 0;
}

static int verifyTime(time_t timeout, process_st *process)
{
    double _timeout = (timeout / 1000000000) + (timeout % 1000000000)/(double)1000000000;    
    return ((process->old + _timeout) > process->update);
}
