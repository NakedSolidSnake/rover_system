#include <lcd16.h>
#include <lcd16_action.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <app.h>

int lcd16_action_select(const char *action, int action_len)
{
    MEM *mem = mem_get();
    if (mem == NULL)
    {
        return 1;
    }

    strncpy( mem->status.lcd16_status.msg_line1, "Test Line 1", strlen("Test Line 1"));
    strncpy( mem->status.lcd16_status.msg_line2, "Test Line 2", strlen("Test Line 2"));
    
    printf("%s\n", action);
    return 0;
}