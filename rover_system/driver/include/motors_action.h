#ifndef __MOTORS_ACTION_H
#define __MOTORS_ACTION_H

#define MOVE            "move"
#define POWER           "power"

#define MOVE_FORWARD    "forward"
#define MOVE_REVERSE    "reverse"
#define MOVE_LEFT       "left"
#define MOVE_RIGHT      "right"
#define MOVE_STOP       "stop"
  

int motors_action_select(const char *action, int action_len);

#endif
