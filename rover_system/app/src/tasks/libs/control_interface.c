#include <control_interface.h>
#include <log/log.h>
#include <context.h>
#include <includes.h>
#include <auxiliar.h>
#include <signal/signal.h>

static Context_st *iContext;

static void init(Context_st *context, IControl *iControl);
static void messageReceived(int s);
static void updateTime(int s);
static void endProcess(int s);

int controlExec(Context_st *context, IControl *iControl)
{
    init(context, iControl);

    while (1)
    {
        if (context->states.message_received)
        {
            memcpy(&context->msg, &context->mem->msg, sizeof(message_st));
            logger(LOGGER_INFO, context->logType, context->msg.command);
            //call command here
            iControl->actionSelect(context->msg.command, strlen(context->msg.command));
            context->queue.queueType = 1;
            snprintf(context->queue.bData, sizeof(context->queue.bData), "$:%04d:%04d:%s:FFFF:#",
                     context->deviceId,
                     (int)strlen(context->msg.command),
                     context->msg.command);

            loggerArgs(LOGGER_INFO, context->logType, "%s send: %s", context->logType, context->queue.bData);

            queue_send(context->mem->queue_server_id, &context->queue, (int)strlen(context->queue.bData) + 1);

            context->states.message_received = 0;
        }

        else if (context->states.update_time)
        {
            update_clock(getpid(), context);
            alarm(PROCESS_CICLE_SECONDS);
        }

        else
        {
            pause();
        }
    }
}

static void init(Context_st *context, IControl *iControl)
{
    if (iControl->hardwareInit)
    {
        iControl->hardwareInit();
    }    

    context->mem = mem_get();
    if (!context->mem)
    {
        logger(LOGGER_INFO, context->logType, "Memory not initialized");
        exit(EXIT_FAILURE);
    }

    context->sema_message.id = -1;
    context->sema_message.sema_count = 1;
    context->sema_message.state = LOCKED;
    context->sema_message.master = SLAVE;

    semaphore_init(&context->sema_message, SEMA_MESSAGE_ID);

    context->sema_update.id = -1;
    context->sema_update.sema_count = 1;
    context->sema_update.state = LOCKED;
    context->sema_update.master = SLAVE;

    semaphore_init(&context->sema_update, SEMA_UPDATE_ID);

    signal_register(messageReceived, SIGMESSAGERECEIVED);
    signal_register(updateTime, SIGUPDATETIME);
    signal_register(updateTime, SIGALRM);
    signal_register(endProcess, SIGTERM);

    logger(LOGGER_INFO, context->logType, "Initialized");

    iContext = context;

    alarm(PROCESS_CICLE_SECONDS);
}

void messageReceived(int s)
{
    iContext->states.message_received = 1;
}

static void updateTime(int s)
{
    iContext->states.update_time = 1;
}

void endProcess(int s)
{
    logger(LOGGER_INFO, iContext->logType, "Unlaunched");
    exit(s);
}