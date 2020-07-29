#include <includes.h>
#include <rover_monitor.h>
#include <app.h>
#include <sys/types.h>

#define ROVER_MONITOR  "ROVER_MONITOR"

static int verifyTime(struct timespec *current, time_t timeout, process_st *process);

int main(int argc, char const *argv[])
{
    struct timespec  current;
    time_t t = 1000000000;
    int i;
    int ret;

    MEM *mem = mem_get();
    if(mem == NULL)
        return -1;

    clock_gettime(CLOCK_MONOTONIC, &current);
    for (i = 0; i < (mem->process_amount - 1); i++)
    {
        process_st *process = &mem->processes[i];
        process->old = process->update = (double)current.tv_sec  + (double)current.tv_nsec /(double)1000000000;        
    }

    while(1)
    {
        sleep(2);        

        for(i = 0; i < (mem->process_amount - 1); i++)
        {   
            process_st *process = &mem->processes[i];       
            ret = verifyTime(&current, t, process);

            if(!ret){                
                process->old = process->update;
                process->miss_count = 0;
            }else{
                           
                if(process->miss_count == 3){
                    printf("Process %s unable to respond\n", process->name);
                    // printf("Killing process %s\n", process->name);
                    // kill(process->pid, SIGKILL);
                    // printf("recover process %s\n", process->name);                    
                    // pid_t pid = fork();
                    // if (pid == 0)
                    // {
                    //     execl(process->name, process->name, NULL);
                    // }
                    // else if (pid > 0)
                    // { 
                    //     usleep(10000);
                    //     signal(SIGCHLD,SIG_IGN); //ignores child signal                       
                    //     process->pid = pid;                        
                    //     memoryWrite(mem, (void *)process, i);
                    //     process->miss_count = 0;                        
                    // }
                }else {
                    printf("send signal to process %s pid %d\n", process->name, process->pid);
                    //wait for response
                    kill(process->pid, SIGUSR1);  
                    process->miss_count ++;   
                }
                
            }
        }
        
    }
    
    return 0;
}

static int verifyTime(struct timespec *current, time_t timeout, process_st *process)
{
    double _timeout = (timeout / 1000000000) + (timeout % 1000000000)/(double)1000000000;    
    double _current = (double)current->tv_sec  + (double)current->tv_nsec /(double)1000000000;
    return ((process->old + _timeout) > process->update);
}