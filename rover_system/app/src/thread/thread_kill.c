#include <app.h>
#include <signal/signal.h>

int main()
{
    MEM *mem = mem_get();
    if(!mem)
        return -1;

    notify_process(mem->thread_pid, SIGINT);
    
    return 0;
}
