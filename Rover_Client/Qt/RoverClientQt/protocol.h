#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>

#define PAYLOAD_LEN   256

class Protocol : public QObject
{
    Q_OBJECT
public:
    explicit Protocol(QObject *parent = nullptr);

    int protocol_set(QString &bOutput, const Protocol &p);
    int protocol_get(Protocol &p, const QString &pBuff);

    void setId(unsigned short id);
    void setSize(unsigned short size);
    void setPayload(QString& payload);
    void setChecksum(unsigned short checksum);

    unsigned short id() const;
    unsigned char size() const;
    char *payload() const;
    unsigned short checkSum() const;

signals:

protected:
    unsigned short mId;
    unsigned char mSize;
    char mPayload[PAYLOAD_LEN];
    unsigned short mCheckSum;
};

#endif // PROTOCOL_H
