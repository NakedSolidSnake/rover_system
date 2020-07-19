#include <lcd16.h>
#include <lcd16_action.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <app.h>

int lcd16_action_select(const char *action, int action_len)
{
    int line;
    char message[16] = {0};

    MEM *mem = mem_get();
    if (mem == NULL)
    {
        return 1;
    }

    sscanf(action, "%d-%s", &line, message);

    if (line == 1)
    {
        memset(mem->status.lcd16_status.msg_line1, 0, sizeof(mem->status.lcd16_status.msg_line1));
        strncpy(mem->status.lcd16_status.msg_line1, message, strlen(message));
    }
    else if (line == 2)
    {
        memset(mem->status.lcd16_status.msg_line2, 0, sizeof(mem->status.lcd16_status.msg_line2));
        strncpy(mem->status.lcd16_status.msg_line2, message, strlen(message));
    }

    printf("%s\n", action);
    return 0;
}