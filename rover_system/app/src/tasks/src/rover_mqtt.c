#include <includes.h>
#include "MQTTClient.h"
#include <log/log.h>
#include <signal/signal.h>
#include <sharedmemory/sharedmemory.h>
#include <rover_mqtt.h>
#include <context.h>

#define ROVER_MQTT "ROVER_MQTT"

#define ADDRESS "localhost"
#define CLIENTID "555333"

#define ROVER_MOTOR_POWER_TOPIC "/rover/motor/power"
#define ROVER_MOTOR_DIR_TOPIC "/rover/motor/direction"
#define ROVER_SERVO_TOPIC "/rover/servo/position"
#define ROVER_LCD16_TOPIC "/rover/lcd16/message"
#define ROVER_ULTRASOUND_TOPIC "/rover/ultrasound/distance"

static Context_st mqtt_context;

static void publishMotor(MQTTClient client);
static void publishServo(MQTTClient client);
static void publishUltrasound(MQTTClient client);
static void publishLCD(MQTTClient client);

static void init(void);
static void update_time(int s);
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

  init();

  for (; loop != 1;)
  {
    //send temperature measurement

    publishMotor(client);
    publishServo(client);
    publishUltrasound(client);
    publishLCD(client);

    if(mqtt_context.states.update_time){

      mqtt_context.states.update_time = 0;
    }

    sleep(3);
  }
  MQTTClient_disconnect(client, 1000);
  MQTTClient_destroy(&client);
  return NULL;
}

static void publishMotor(MQTTClient client)
{
  char data[10] = {0};
  snprintf(data, 10, "%d", mqtt_context.mem->status.motor_status.direction);
  publish(client, ROVER_MOTOR_DIR_TOPIC, (char *)data);
  memset(data, 0, sizeof(data));
  snprintf(data, 10, "%d", mqtt_context.mem->status.motor_status.power);
  publish(client, ROVER_MOTOR_POWER_TOPIC, (char *)data);
}

static void publishServo(MQTTClient client)
{
  char data[10] = {0};
  snprintf(data, 10, "%d", mqtt_context.mem->status.servo_status.position);
  publish(client, ROVER_SERVO_TOPIC, (char *)data);
}


static void publishUltrasound(MQTTClient client)
{
  char data[10] = {0};
  snprintf(data, 10, "%0.2f", mqtt_context.mem->status.ultrasound_status.distance);
  publish(client, ROVER_ULTRASOUND_TOPIC, (char *)data);
}

static void publishLCD(MQTTClient client)
{
  char data[40] = {0};
  lcd16_status_st *s = &mqtt_context.mem->status.lcd16_status;
  snprintf(data, 40, "%s\n%s", s->msg_line1, s->msg_line2);
  publish(client, ROVER_LCD16_TOPIC, (char *)data);
}


static void init(void)
{
  memset(&mqtt_context, 0, sizeof(Context_st));

  mqtt_context.mem = mem_get();
  if(!mqtt_context.mem)
  {
    logger(LOGGER_INFO, ROVER_MQTT, "Memory not initialized");
    exit(EXIT_FAILURE);
  }

  signal_register(update_time, SIGUPDATETIME);
  signal_register(end_mqtt, SIGTERM);

  logger(LOGGER_INFO, ROVER_MQTT, "MQTT initialized");
}

static void update_time(int s)
{
  mqtt_context.states.update_time = 1;
}

static void end_mqtt(int s)
{
  logger(LOGGER_INFO, ROVER_MQTT, "MQTT Termined");
  loop = 1;
}