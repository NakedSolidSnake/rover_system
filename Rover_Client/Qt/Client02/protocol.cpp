#include "protocol.h"
#include <stdlib.h>

Protocol::Protocol(QObject *parent) : QObject(parent),
  mId(0),
  mSize(0),
  mCheckSum(0)
{
    memset(mPayload, 0, sizeof(mPayload));
}

int Protocol::protocol_set(QString &bOutput, const Protocol &p)
{
    if(bOutput.isNull())
        return -1;
    bOutput.sprintf("%04d:%04d:%s:%04d", p.id(), p.size(), p.payload(), p.checkSum());

    return 0;
}

int Protocol::protocol_get(Protocol &p, const QString &pBuff)
{
    if(pBuff.isNull())
        return -1;
    QStringList list = pBuff.split(":");

    p.setId(list[0].toUShort());
    p.setSize(list[1].toUShort());
    p.setPayload(list[2]);
    p.setChecksum(list[3].toUShort());
    return 0;
}

void Protocol::setId(unsigned short id)
{
    this->mId = id;
}

void Protocol::setSize(unsigned short size)
{
    this->mSize = size;
}

void Protocol::setPayload(QString &payload)
{
    QByteArray ba = payload.toLocal8Bit();
    memcpy(this->mPayload, ba.data(), sizeof(this->mPayload));
}

void Protocol::setChecksum(unsigned short checksum)
{
    this->mCheckSum = checksum;
}

unsigned short Protocol::id() const
{
    return mId;
}

unsigned char Protocol::size() const
{
    return mSize;
}

char *Protocol::payload() const
{
    return (char *)&mPayload;
}

unsigned short Protocol::checkSum() const
{
    return mCheckSum;
}

