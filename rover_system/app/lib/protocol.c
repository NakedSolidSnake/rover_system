#include <string.h>
#include <protocol.h>

int protocol_setId(protocol_t *p, const char *id)
{
  if(!p)
    return -1;

  strncpy(p->id, id, ID_LEN);
  return 0;
}

int protocol_setSize(protocol_t *p, const char *size)
{
  if(!p)
    return -1;

  strncpy(p->size, size, SIZE_LEN);
  return 0;
}

int protocol_setPayload(protocol_t *p, const char *payload)
{
  if(!p)
    return -1;
  strncpy(p->payload, payload, PAYLOAD_LEN);
  return 0;
}

int protocol_setCheckSum(protocol_t *p, const char *checksum)
{
  if(!p)
    return -1;

  strncpy(p->chk, checksum, CHKSUM_LEN);
  return 0;
}

int protocol_get(protocol_t *p, const char *pBuff, int bSize)
{
  char *pOffset = pBuff;
  if(!p)
    return -1;

  memset(p, 0, sizeof(protocol_t));

  memcpy(p->id, pOffset, ID_LEN);
  pOffset = pOffset + sizeof(p->id);
  memcpy(p->size, pOffset, SIZE_LEN);
  pOffset = pOffset + sizeof(p->size);
  memcpy(p->payload, pOffset, PAYLOAD_LEN);
  pOffset = pOffset + sizeof(p->payload);
  memcpy(p->chk, pOffset, CHKSUM_LEN);

  return 0;
}

int protocol_set(char *bOutput, int bSize, const protocol_t *p)
{
  char *pBuff = NULL;

  if(!bOutput)
    return -1;

  if(bSize < sizeof(protocol_t))
    return 1;

  pBuff = bOutput;
  memcpy(pBuff, p->id, ID_LEN);
  pBuff = pBuff + sizeof(p->id);
  memcpy(pBuff, p->size, SIZE_LEN);
  pBuff = pBuff + sizeof(p->size);
  memcpy(pBuff, p->payload, PAYLOAD_LEN);
  pBuff = pBuff + sizeof(p->payload);
  memcpy(pBuff, p->chk, CHKSUM_LEN);

  return 0;
}
