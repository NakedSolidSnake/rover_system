#include <app.h>

int get_pid(pid_t pid)
{
    int i;
    MEM *mem = mem_get();
    if(!mem){
        return -1;
    }

    for(i = 0; i < mem->process_amount; i++)
    {
        if(pid == (pid_t)mem->processes[i].pid)
        {
            return i;
        }
    }

    return -1;
}