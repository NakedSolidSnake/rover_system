#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "MQTTClient.h"
#include <log/log.h>
#include <signal/signal.h>
#include <signal.h>
#include <sharedmemory/sharedmemory.h>
#include <app.h>

#define ROVER_MQTT   "ROVER_MQTT"

#define ADDRESS     "localhost"
#define CLIENTID    "555333"

#define ROVER_MOTOR_POWER_TOPIC     "/rover/motor/power"
#define ROVER_MOTOR_DIR_TOPIC       "/rover/motor/direction"
#define ROVER_SERVO_TOPIC           "/rover/servo/position"
#define ROVER_LCD16_TOPIC           "/rover/lcd16/message"
#define ROVER_ULTRASOUND_TOPIC      "/rover/ultrasound/distance"

static void publishMotor(MQTTClient client, const char *command);
static void publishMotorPower(MQTTClient client, const char *command);
static void publishMotorDirection(MQTTClient client, const char *command);
static void publishServo(MQTTClient client, const char *command);
static void publicServoPosition(MQTTClient client, const char *command);


static void end_mqtt(int s);

static int loop = 0;

void publish(MQTTClient client, char* topic, char* payload) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(pubmsg.payload);
    pubmsg.qos = 0;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
    loggerArgs(LOGGER_INFO, ROVER_MQTT, "Message '%s' with delivery token %d delivered", payload, token);
}

int main(int argc, char* argv[]) {
    MEM *mem = NULL;
    MQTTClient client;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    // conn_opts.username = "<<tenant_ID>>/<<username>>";
    // conn_opts.password = "<<password>>";

    int rc;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }

    signal_register(end_mqtt, SIGTERM);

    logger(LOGGER_INFO, ROVER_MQTT, "MQTT Started");

    mem = mem_get();
    if(mem == NULL)
    {
        return 1;
    }
    
    for (;loop != 1;) {
       //send temperature measurement
        generic_st *m = &mem->motor;
        generic_st *s = &mem->servo;
        
        publishMotor(client, m->command);
        publishServo(client, s->command);        
        
        sleep(3);
    }
    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    return rc;
}

static void publishMotor(MQTTClient client, const char *command)
{
  char action[10] = {0};
  char data[10] = {0};
  char *p = NULL;  

  sscanf(command, "%s", action);

  if (!strncmp(command, "move", strlen("move")))
  {
    p = (char *)(command + strlen(action) + 1); //points to next string
    strncpy(data, p, 10);
    //call functions
    publishMotorDirection(client, p);
  }
  else if (!strncmp(command, "power", strlen("power")))
  {
    p = (char *)(command + strlen(action) + 1); //points to next string
    strncpy(data, p, 10);
    //call function
    publishMotorPower(client, data);
  }
}

static void publishMotorPower(MQTTClient client, const char *command)
{
    publish(client, ROVER_MOTOR_POWER_TOPIC, (char *)command);
}

static void publishMotorDirection(MQTTClient client, const char *command)
{
    publish(client, ROVER_MOTOR_DIR_TOPIC, (char *)command);
}

static void publishServo(MQTTClient client, const char *command)
{
  char action[10] = {0};
  char data[10] = {0};
  char *p = NULL;

  sscanf(command, "%s", action);

  if (!strncmp(command, "graus", strlen("graus")))
  {
    p = (char *)(command + strlen(action) + 1); //points to next string
    strncpy(data, p, 10);
    //call functions
    publicServoPosition(client, p);
  }  
}

static void publicServoPosition(MQTTClient client, const char *command)
{
    publish(client, ROVER_SERVO_TOPIC, (char *)command);
}

static void end_mqtt(int s)
{
  logger(LOGGER_INFO, ROVER_MQTT, "MQTT Termined");
  loop = 1;
}