#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "MQTTClient.h"
#include <log/log.h>
#include <signal/signal.h>
#include <signal.h>
#include <sharedmemory/sharedmemory.h>
#include <app.h>
#include <rover_mqtt.h>

#define ROVER_MQTT "ROVER_MQTT"

#define ADDRESS "localhost"
#define CLIENTID "555333"

#define ROVER_MOTOR_POWER_TOPIC "/rover/motor/power"
#define ROVER_MOTOR_DIR_TOPIC "/rover/motor/direction"
#define ROVER_SERVO_TOPIC "/rover/servo/position"
#define ROVER_LCD16_TOPIC "/rover/lcd16/message"
#define ROVER_ULTRASOUND_TOPIC "/rover/ultrasound/distance"

static void publishMotor(MQTTClient client, MEM *mem);
// static void publishMotor(MQTTClient client, const char *command);
static void publishMotorPower(MQTTClient client, const char *command);
static void publishMotorDirection(MQTTClient client, const char *command);

static void publishServo(MQTTClient client, MEM *mem);
// static void publishServo(MQTTClient client, const char *command);
static void publicServoPosition(MQTTClient client, const char *command);

// static void publishUltrasound(MQTTClient client, const char *command);
static void publishUltrasound(MQTTClient client, MEM *mem);
static void publishUltrasoundDistance(MQTTClient client, const char *command);

static void publishLCD(MQTTClient client, MEM *mem);
// static void publishLCD(MQTTClient client, const char *command);
static void publishLCDMessageLine1(MQTTClient client, const char *command);
static void publishLCDMessageLine2(MQTTClient client, const char *command);

static void end_mqtt(int s);

static int loop = 0;

void publish(MQTTClient client, char *topic, char *payload)
{
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

#ifdef PROCESS
int main(int argc, char const *argv[])
{
  (void)rover_mqtt(NULL);
  return 0;
}
#endif

void *rover_mqtt(void *args)
{
  (void)args;
  MEM *mem = NULL;
  MQTTClient client;
  MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
  MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
  // conn_opts.username = "<<tenant_ID>>/<<username>>";
  // conn_opts.password = "<<password>>";

  int rc;
  if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
  {
    printf("Failed to connect, return code %d\n", rc);
    exit(-1);
  }

  signal_register(end_mqtt, SIGTERM);

  logger(LOGGER_INFO, ROVER_MQTT, "MQTT Started");

  mem = mem_get();
  if (mem == NULL)
  {
    return 1;
  }

  for (; loop != 1;)
  {
    //send temperature measurement
    generic_st *m = &mem->motor;
    generic_st *s = &mem->servo;

    // publishMotor(client, m->command);
    publishMotor(client, mem);
    publishServo(client, mem);
    publishUltrasound(client, mem);
    publishLCD(client, mem);
    // publishServo(client, s->command);

    sleep(3);
  }
  MQTTClient_disconnect(client, 1000);
  MQTTClient_destroy(&client);
  return rc;
}

static void publishMotor(MQTTClient client, MEM *mem)
{
  char data[10] = {0};
  snprintf(data, 10, "%d", mem->status.motor_status.direction);
  publish(client, ROVER_MOTOR_DIR_TOPIC, (char *)data);
  memset(data, 0, sizeof(data));
  snprintf(data, 10, "%d", mem->status.motor_status.power);
  publish(client, ROVER_MOTOR_POWER_TOPIC, (char *)data);
}

// static void publishMotor(MQTTClient client, const char *command)
// {
//   char action[10] = {0};
//   char data[10] = {0};
//   char *p = NULL;

//   sscanf(command, "%s", action);

//   if (!strncmp(command, "move", strlen("move")))
//   {
//     p = (char *)(command + strlen(action) + 1); //points to next string
//     strncpy(data, p, 10);
//     //call functions
//     publishMotorDirection(client, p);
//   }
//   else if (!strncmp(command, "power", strlen("power")))
//   {
//     p = (char *)(command + strlen(action) + 1); //points to next string
//     strncpy(data, p, 10);
//     //call function
//     publishMotorPower(client, data);
//   }
// }

static void publishMotorPower(MQTTClient client, const char *command)
{
  publish(client, ROVER_MOTOR_POWER_TOPIC, (char *)command);
}

static void publishMotorDirection(MQTTClient client, const char *command)
{
  publish(client, ROVER_MOTOR_DIR_TOPIC, (char *)command);
}

// static void publishServo(MQTTClient client, const char *command)
static void publishServo(MQTTClient client, MEM *mem)
{
  char action[10] = {0};
  // char data[10] = {0};
  // char *p = NULL;

  // sscanf(command, "%s", action);

  // if (!strncmp(command, "graus", strlen("graus")))
  // {
  //   p = (char *)(command + strlen(action) + 1); //points to next string
  //   strncpy(data, p, 10);
  //   //call functions
  //   publicServoPosition(client, p);
  // }
  char data[10] = {0};
  snprintf(data, 10, "%d", mem->status.servo_status.position);
  publish(client, ROVER_SERVO_TOPIC, (char *)data);
}

static void publicServoPosition(MQTTClient client, const char *command)
{
  publish(client, ROVER_SERVO_TOPIC, (char *)command);
}

static void publishUltrasound(MQTTClient client, MEM *mem)
{
  char data[10] = {0};
  snprintf(data, 10, "%0.2f", mem->status.ultrasound_status.distance);
  publish(client, ROVER_ULTRASOUND_TOPIC, (char *)data);
}

static void publishLCD(MQTTClient client, MEM *mem)
{
  char data[40] = {0};
  lcd16_status_st *s = &mem->status.lcd16_status;
  snprintf(data, 40, "%s\n%s", s->msg_line1, s->msg_line2);
  publish(client, ROVER_LCD16_TOPIC, (char *)data);
}

static void end_mqtt(int s)
{
  logger(LOGGER_INFO, ROVER_MQTT, "MQTT Termined");
  loop = 1;
}