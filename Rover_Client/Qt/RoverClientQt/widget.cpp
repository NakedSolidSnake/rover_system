#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QDataStream>
#include "protocol.h"
#include <QLCDNumber>
#include <QSlider>
#include <QKeyEvent>

#define SERVO_LEFT_POS      29
#define SERVO_CENTER_POS    73
#define SERVO_RIGHT_POS     118

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget),
      mSocket(new QTcpSocket(this)),
      mSocketReady(false)
{
    ui->setupUi(this);

    connect(ui->leIP, &QLineEdit::editingFinished, this, &Widget::on_saveHostSettings);
    connect(ui->lePort, &QLineEdit::editingFinished, this, &Widget::on_savePortSettings);

    setWindowTitle("Client | Data Sender");

    buttonState(true);

    loadSettings();

    ui->leSend->setText("0001:0004:size:FFFF");

    ui->vsPower->setMaximum(1000);
    ui->dialServo->setMinimum(29);
    ui->dialServo->setMaximum(118);
}

Widget::~Widget()
{
    mSocket->close();
    delete ui;
}

void Widget::socketReady()
{
    mSocketReady = true;
}

void Widget::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << socketState;
}

void Widget::on_btConnect_clicked()
{
    QString ip = ui->leIP->text();
    int port = ui->lePort->text().toInt();

    if(ip.isNull() || ip.isEmpty() || port <= 0)
        return ;

    mSocket->connectToHost(ip, static_cast<quint16>(port));

    connect(mSocket, &QTcpSocket::connected, this, &Widget::socketReady);
    connect(mSocket, &QTcpSocket::stateChanged,this,&Widget::stateChanged);
    connect(ui->vsPower, &QSlider::valueChanged, this, &Widget::on_powerChanged);
    connect(ui->dialServo, &QDial::valueChanged, this, &Widget::on_servoSetPosition);

    out.setDevice(mSocket);

    buttonState(false);
    setKeySequence();
}

void Widget::on_btDisconnect_clicked()
{
    mSocket->close();
    buttonState(true);
}

void Widget::on_btSend_clicked()
{
    QString buffer = ui->leSend->text();
    if(buffer.isEmpty() || buffer.isNull()) //launch exception
        return;

    qDebug() << buffer;

    out.writeRawData(buffer.toLocal8Bit(), buffer.size());
}

void Widget::buttonState(bool state)
{
    ui->leIP->setEnabled(state);
    ui->lePort->setEnabled(state);
    ui->btConnect->setEnabled(state);

    ui->btDisconnect->setEnabled(!state);
    ui->gbSend->setEnabled(!state);

    ui->gbCommand->setEnabled(!state);
    ui->gbServo->setEnabled(!state);

}

void Widget::setKeySequence()
{
    ui->btForward->setShortcut(QKeySequence(Qt::Key_Up));
    ui->btBackwards->setShortcut(QKeySequence(Qt::Key_Down));
    ui->btLeft->setShortcut(QKeySequence(Qt::Key_Left));
    ui->btRight->setShortcut(QKeySequence(Qt::Key_Right));
    ui->btStop->setShortcut(QKeySequence(Qt::Key_0));

    ui->btLess->setShortcut(QKeySequence(Qt::Key_Minus));
    ui->btPlus->setShortcut(QKeySequence(Qt::Key_Plus));

}

void Widget::loadSettings()
{
    QMap<QString, QString> keys;
    keys["host"] = "";
    keys["port"] = "";
    persistConfig.getData("config", keys);

    QMapIterator<QString, QString> it(keys);

    while(it.hasNext()){
        it.next();

        if(it.key() == "host"){
            if(!it.value().isEmpty()){
                 ui->leIP->setText(it.value());
            }else {
                ui->leIP->setText("localhost");
            }
        }

        if(it.key() == "port"){
            if(!it.value().isEmpty()){
                 ui->lePort->setText(it.value());
            }else {
                ui->lePort->setText("1234");
            }
        }
    }
}

void Widget::on_btForward_clicked()
{
    QString command = "$:0000:0012:move forward:FFFF:#";
    out.writeRawData(command.toLocal8Bit(), command.size());
}

void Widget::on_btStop_clicked()
{
    QString command = "$:0000:0009:move stop:FFFF:#";
    out.writeRawData(command.toLocal8Bit(), command.size());
}

void Widget::on_btLeft_clicked()
{
    QString command = "$:0000:0009:move left:FFFF:#";
    out.writeRawData(command.toLocal8Bit(), command.size());
}

void Widget::on_btRight_clicked()
{
    QString command = "$:0000:0010:move right:FFFF:#";
    out.writeRawData(command.toLocal8Bit(), command.size());
}

void Widget::on_btBackwards_clicked()
{
    QString command = "$:0000:0012:move reverse:FFFF:#";
    out.writeRawData(command.toLocal8Bit(), command.size());
}

void Widget::on_powerChanged(int value)
{
    QString command = QString("$:0000:0012:power %1:FFFF:#").arg(value);
    ui->lcdPower->display(value);
    out.writeRawData(command.toLocal8Bit(), command.size());
}

void Widget::on_btPlus_clicked()
{
    int value = ui->vsPower->value() + 1;
    ui->vsPower->setValue(value);
}

void Widget::on_btLess_clicked()
{
    int value = ui->vsPower->value() - 1;
    ui->vsPower->setValue(value);
}

void Widget::on_btServoRight_clicked()
{
    QString command = "$:0001:0009:set right:FFFF:#";
    ui->dialServo->setValue(SERVO_RIGHT_POS);
}

void Widget::on_btServoCenter_clicked()
{
    QString command = "$:0001:0010:set center:FFFF:#";
    ui->dialServo->setValue(SERVO_CENTER_POS);
}

void Widget::on_btServoLeft_clicked()
{
    QString command = "$:0001:0008:set left:FFFF:#";
    ui->dialServo->setValue(SERVO_LEFT_POS);
}

void Widget::on_servoSetPosition(int value)
{
    QString command = QString("$:0001:0012:graus %1:FFFF:#").arg(value);
    out.writeRawData(command.toLocal8Bit(), command.size());
}

void Widget::on_saveHostSettings()
{
    QMap<QString, QString> key;
    key["host"] = ui->leIP->text();
    persistConfig.save("config", key);
}

void Widget::on_savePortSettings()
{
    QMap<QString, QString> key;
    key["port"] = ui->lePort->text();
    persistConfig.save("config", key);
}
