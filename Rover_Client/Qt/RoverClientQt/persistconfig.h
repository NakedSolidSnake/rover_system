#ifndef PERSISTCONFIG_H
#define PERSISTCONFIG_H

#include <QObject>
#include <QSettings>

class PersistConfig : public QObject
{
    Q_OBJECT
public:
    explicit PersistConfig(QObject *parent = nullptr);
    void save(QString group, QMap<QString, QString> &keys);
    void getData(QString group, QMap<QString, QString> &keys);

signals:

private:
    QSettings *m_Settings;
};

#endif // PERSISTCONFIG_H
