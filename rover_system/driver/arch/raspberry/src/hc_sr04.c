#include <stdlib.h>
#include <hc_sr04.h>
#include <wiringPi.h>

int HC_SR04_init(void)
{
  int ret = -1;
  ret = wiringPiSetupGpio();
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);

  /* TRIGGER pin must be starting LOW */
  digitalWrite(TRIGGER, LOW);
  delay(30);

  return EXIT_SUCCESS;
}

int HC_SR04_getCM(void)
{
  /* Send TRIGGER pulse */
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(20);
  digitalWrite(TRIGGER, LOW);


  /* Wait ECHO pin start */
  while(digitalRead(ECHO) == LOW);

  /* Wait ECHO end */
  long startTime = micros();
  while(digitalRead(ECHO) == HIGH);
  long travelTime = micros() - startTime;

  /* Convert to distance in cm */
  int distance = travelTime /58;

  return distance;
}
