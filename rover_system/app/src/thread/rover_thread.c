#include <pthread.h>
#include <unistd.h>
#include <common.h>
#include <signal/signal.h>
#include <signal.h>
#include <rover_lcd16_control.h>
#include <rover_manager.h>
#include <rover_motor_control.h>
#include <rover_mqtt.h>
#include <rover_server.h>
#include <rover_servo_control.h>
#include <rover_ultrasound_control.h>
#include <rover_webserver.h>
#include <app.h>

static int running = 1;
void end_thread(int sig);

typedef struct threadList
{
    pthread_t thread;
    Function_t function;
    const char *name;
}threadList_st;

static threadList_st tList[] = 
{
    {.function = rover_lcd16_control,      .name = "rover_lcd16"},
    {.function = rover_manager,            .name = "rover_manager"},
    {.function = rover_motor_control,      .name = "rover_motor"},
    {.function = rover_mqtt,               .name = "rover_mqtt"},
    {.function = rover_server,             .name = "rover_server"},
    {.function = rover_servo_control,      .name = "rover_servo"},
    {.function = rover_ultrasound_control, .name = "rover_ultrasnd"},
    {.function = rover_webserver,          .name = "rover_webserver"}
};

static int threads_amount = sizeof(tList)/sizeof(tList[0]);


int main(int argc, char const *argv[])
{

    MEM *mem = NULL;

    if(mem_init() < 0){
        return -1;
    }

    if(ipc_init() < 0){
        return -1;
    }

    signal_register(end_thread, SIGINT);

    for(int i = 0; i < threads_amount; i++){
        pthread_create(&tList[i].thread, NULL, tList[i].function, NULL);
        pthread_setname_np(tList[i].thread, tList[i].name);
    }

    mem = mem_get();
    if(!mem)
        return -1;

    mem->thread_pid = getpid();

    while(running){
        pause();
    }   

    mem_denit();

    return 0;
}


void end_thread(int sig)
{
    running = 0;
}
