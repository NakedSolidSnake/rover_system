#include "protocol.h"

Protocol::Protocol() :
    m_id(""),
    m_size(""),
    m_payload(""),
    m_chk("")
{

}

Protocol::Protocol(QByteArray id, QByteArray size, QByteArray payload, QByteArray checksum)
{

   this->m_size = size;
   this->m_payload = payload;
   this->m_chk = checksum;
}


void Protocol::setId(const QByteArray &id)
{
    this->m_id = id;
}

void Protocol::setSize(const QByteArray &size)
{
    this->m_size = size;
}

void Protocol::setPayload(const QByteArray &payload)
{
    this->m_payload = payload;
}

void Protocol::setCheckSum(const QByteArray &checksum)
{
    this->m_chk = checksum;
}

void Protocol::set(const QByteArray &output)
{

    //fill output with protocol

//    this->m_id = protocol.;
//    this->m_size = size;
//    this->m_payload = payload;
//    this->m_chk = checksum;
}

Protocol Protocol::get(const QByteArray &input)
{
    //parse buffer to protocol class
    Protocol p;
    return p;
}
