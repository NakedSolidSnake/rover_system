#include <auxiliar.h>

void update_clock(pid_t pid, Context_st *context)
{
    int index = get_index_from_pid(pid);
      if(index >= 0 &&  index < context->mem->process_amount)
      {
        clock_gettime(CLOCK_MONOTONIC, &context->current);        
        // if (semaphore_lock(&motor_context.sema_update) == 0)
        {
          context->mem->processes[index].update = (time_t)((double)context->current.tv_sec + (double)context->current.tv_nsec/(double)1000000000);
          // semaphore_unlock(&motor_context.sema_update);
        }
      }
      context->states.update_time = 0;
}

  