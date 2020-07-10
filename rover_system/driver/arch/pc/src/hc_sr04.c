#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <hc_sr04.h>

int HC_SR04_init(void)
{
  printf("HC_SR04 initialized.\n");
  srand(time(NULL));
  return EXIT_SUCCESS;
}

float HC_SR04_getCM(void)
{
  return (rand() % 100) + 50;
}
