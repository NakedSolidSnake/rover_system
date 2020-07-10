#include <ultrasound_action.h>
#include <hc_sr04.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <app.h>

int ultrasound_action_select(const char *action, int action_len)
{
    MEM *mem = mem_get();
    if (mem == NULL)
    {
        return 1;
    }

    mem->status.ultrasound_status.distance = HC_SR04_getCM();

    printf("%s\n", action);
    return 0;
}