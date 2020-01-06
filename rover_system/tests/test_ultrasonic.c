#include <stdio.h>
#include <stdlib.h>
#include <hc_sr04.h>

int main()
{
  int ret = -1;
  int valueRead = 0;
  ret =  HC_SR04_init();
  if(ret != EXIT_SUCCESS){
    return EXIT_FAILURE;
  }

  while(1){
    valueRead = HC_SR04_getCM();
    printf("Value read in cm = %d\n", valueRead);
    sleep(2);
  }

  return EXIT_SUCCESS;
}
