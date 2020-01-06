#ifndef __SERVO_ACTION_H
#define __SERVO_ACTION_H

#define GRAUS               "graus"
#define SET                 "set"

#define MOVE_CENTER         "center"
#define MOVE_LEFT           "left"
#define MOVE_RIGHT          "right"

#define LEFT        29
#define RIGHT       118
#define CENTER      73

int servo_action_select(const char *action, int action_len);

#endif
