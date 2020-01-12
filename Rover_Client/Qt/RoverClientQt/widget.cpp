#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QDataStream>
#include "protocol.h"
#include <QLCDNumber>
#include <QSlider>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget),
      mSocket(new QTcpSocket(this)),
      mSocketReady(false)

{
    ui->setupUi(this);
    setWindowTitle("Client | Data Sender");

    buttonState(true);

    ui->leIP->setText("localhost");
    ui->lePort->setText("8080");
    ui->leSend->setText("0001:0004:size:FFFF");

    ui->vsPower->setMaximum(1000);
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
    out.setDevice(mSocket);

    buttonState(false);
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

}

void Widget::on_btForward_clicked()
{
    QString command = "0000:0012:move forward:FFFF";
    out.writeRawData(command.toLocal8Bit(), command.size());
}

void Widget::on_btStop_clicked()
{
    QString command = "0000:0009:move stop:FFFF";
    out.writeRawData(command.toLocal8Bit(), command.size());
}

void Widget::on_btLeft_clicked()
{
    QString command = "0000:0009:move left:FFFF";
    out.writeRawData(command.toLocal8Bit(), command.size());
}

void Widget::on_btRight_clicked()
{
    QString command = "0000:0010:move right:FFFF";
    out.writeRawData(command.toLocal8Bit(), command.size());
}

void Widget::on_btBackwards_clicked()
{
    QString command = "0000:0012:move reverse:FFFF";
    out.writeRawData(command.toLocal8Bit(), command.size());
}

void Widget::on_powerChanged(int value)
{
    QString command = QString("0000:0012:power %1:FFFF").arg(value);
    ui->lcdPower->display(value);
    out.writeRawData(command.toLocal8Bit(), command.size());
}
