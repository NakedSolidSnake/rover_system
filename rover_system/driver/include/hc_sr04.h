#ifndef __HC_SR04_H
#define __HC_SR04_H

#define TRIGGER     24
#define ECHO        25

int HC_SR04_init(void);
int HC_SR04_getCM(void);

#endif
