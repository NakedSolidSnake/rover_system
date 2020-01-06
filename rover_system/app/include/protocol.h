#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#define ID_LEN        4
#define SIZE_LEN      4
#define PAYLOAD_LEN   64 
#define CHKSUM_LEN    4

typedef struct protocol
{
  char id[ID_LEN + 1];
  char size[SIZE_LEN + 1];
  char payload[PAYLOAD_LEN + 1];
  char chk[CHKSUM_LEN + 1];
}protocol_t;

int protocol_setId(protocol_t *p, const char *id);
int protocol_setSize(protocol_t *p, const char *size);
int protocol_setPayload(protocol_t *p, const char *payload);
int protocol_setCheckSum(protocol_t *p, const char *checksum);

int protocol_set(char *bOutput, int bSize, const protocol_t *p);
int protocol_get(protocol_t *p, const char *pBuff, int bSize);

#endif
