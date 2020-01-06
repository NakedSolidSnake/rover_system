#ifndef __CLIENTTCP_H
#define __CLIENTTCP_H

#define MAX_PORT_LEN    5
#define MAX_IPADDR_LEN  20


typedef struct clienttcp
{
  char port[MAX_PORT_LEN];
  char ipAddr[MAX_IPADDR_LEN];
}clienttcp_t;

#endif
