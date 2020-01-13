#include "persistconfig.h"
#include <QCoreApplication>
#include <QMapIterator>


PersistConfig::PersistConfig(QObject *parent) : QObject(parent)
{
    QCoreApplication::setOrganizationName("Rover Systems");
    QCoreApplication::setOrganizationDomain("roversystems.com");
    QCoreApplication::setApplicationName("Rover System");
    m_Settings = new QSettings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
}

void PersistConfig::save(QString group, QMap<QString, QString> &keys)
{
    QMapIterator<QString, QString> i(keys);
    m_Settings->beginGroup(group);

    while(i.hasNext()){
        i.next();
        m_Settings->setValue(i.key(), i.value());
    }

    m_Settings->endGroup();

    m_Settings->sync();
}

void PersistConfig::getData(QString group, QMap<QString, QString> &keys)
{
    QMapIterator<QString, QString> i(keys);

    m_Settings->beginGroup(group);

    while(i.hasNext()){
        i.next();
        QString recv = m_Settings->value(i.key()).toString();
        if(!recv.isEmpty()){
            keys[i.key()] = recv;
        }
    }

    m_Settings->endGroup();
}
