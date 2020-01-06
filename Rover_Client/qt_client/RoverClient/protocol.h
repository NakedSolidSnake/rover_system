#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QString>
#include <QByteArray>

#define ID_LEN        4
#define SIZE_LEN      4
#define PAYLOAD_LEN   64
#define CHKSUM_LEN    4

class Protocol
{

public:
    Protocol();
    Protocol(QByteArray id, QByteArray size, QByteArray payload, QByteArray checksum);

    void setId(const QByteArray &id);
    void setSize(const QByteArray &size);
    void setPayload(const QByteArray &payload);
    void setCheckSum(const QByteArray &checksum);

    void set(const QByteArray &output);
    Protocol get(const QByteArray &input);

private:
    QByteArray m_id;
    QByteArray m_size;
    QByteArray m_payload;
    QByteArray m_chk;
};

#endif // PROTOCOL_H
