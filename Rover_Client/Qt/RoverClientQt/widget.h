#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QDataStream>
#include "persistconfig.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    enum{
      MOTOR_ID = 0,
      SERVO_ID,
      ULTRASOUND_ID,
      LCD_ID
    };

private:
    void buttonState(bool state);
    void setKeySequence(void);
    void loadSettings();

private slots:
    void socketReady();
    void stateChanged(QAbstractSocket::SocketState socketState);
    void on_btConnect_clicked();
    void on_btDisconnect_clicked();
    void on_btSend_clicked();
    void on_btForward_clicked();
    void on_btStop_clicked();
    void on_btLeft_clicked();
    void on_btRight_clicked();
    void on_btBackwards_clicked();
    void on_powerChanged(int value);
    void on_btPlus_clicked();
    void on_btLess_clicked();
    void on_btServoRight_clicked();
    void on_btServoCenter_clicked();
    void on_btServoLeft_clicked();
    void on_servoSetPosition(int value);
    void on_saveHostSettings(void);
    void on_savePortSettings(void);
    void on_cbPeriodic_toggled(bool checked);
    void on_btRead_clicked();
    void on_btSendLCD_clicked();
    void on_getDistance();
    void on_ReadyRead();

private:
    Ui::Widget *ui;
    QTcpSocket *mSocket;
    bool mSocketReady;
    QDataStream out;
    PersistConfig persistConfig;
    QTimer mTimerUltrasound;
};
#endif // WIDGET_H
