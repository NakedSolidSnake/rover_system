#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <protocol.h>

enum protocol_identifier
{
  PROT_ID = 0,
  PROT_SIZE,
  PROT_PAYLOAD,
  PROT_CHECKSUM
};

struct protocol_map
{
  int idx;
  int (*cb_fill)(protocol_t *prot, const char *bFrag);
};

static int cb_fill_id(protocol_t *prot, const char *bFrag);
static int cb_fill_size(protocol_t *prot, const char *bFrag);
static int cb_fill_payload(protocol_t *prot, const char *bFrag);
static int cb_fill_checkcsum(protocol_t *prot, const char *bFrag);

int protocol_umount(protocol_t *p, const char *pBuff, int bSize)
{
  int i = 0;

  struct protocol_map p_map[] =
  {
    {.idx = PROT_ID, cb_fill_id},
    {.idx = PROT_SIZE, cb_fill_size},
    {.idx = PROT_PAYLOAD, cb_fill_payload},
    {.idx = PROT_CHECKSUM, cb_fill_checkcsum},    
  };

  char *pB = strtok((char *)pBuff, ":");
  if(strncmp(pB, "$", 1))
    return 1;

  pB = strtok(NULL, ":");

  while(pB != NULL)
  {
    if ( p_map[i].cb_fill(p, pB) )
      return i;
    pB = strtok(NULL, ":");

    if(!strncmp(pB, "#", 1))
      break;

    i++;
  }
  
   return 0;
}

int protocol_mount(const protocol_t *p, char *bOutput, int bSize)
{
  snprintf(bOutput, bSize, "%04d:%04d:%*s:%04d", p->id, p->size, p->size, p->payload, p->checksum);
  return 0;  
}

static int cb_fill_id(protocol_t *prot, const char *bFrag)
{
  prot->id = atoi(bFrag);
  return 0;
}

static int cb_fill_size(protocol_t *prot, const char *bFrag)
{
  prot->size = atoi(bFrag);
  return 0;
}

static int cb_fill_payload(protocol_t *prot, const char *bFrag)
{
  strncpy(prot->payload, bFrag, prot->size);
  return 0;
}

static int cb_fill_checkcsum(protocol_t *prot, const char *bFrag)
{
  prot->checksum = (unsigned short) strtoul (bFrag, NULL, 16);
  return 0;
}