#include <stdio.h>
#include <stdlib.h>
#include <hc_sr04.h>

int HC_SR04_init(void)
{
  printf("HC_SR04 initialized.\n");
  return EXIT_SUCCESS;
}

int HC_SR04_getCM(void)
{
  return rand();
}
