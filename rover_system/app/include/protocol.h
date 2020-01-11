#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#define PAYLOAD_LEN   256 

typedef struct protocol
{
  unsigned short id;
  unsigned char size;
  char payload[PAYLOAD_LEN];
  unsigned short checksum;
}protocol_t;

int protocol_umount(protocol_t *p, const char *pBuff, int bSize);
int protocol_mount(const protocol_t *p, char *bOutput, int bSize);

#endif
