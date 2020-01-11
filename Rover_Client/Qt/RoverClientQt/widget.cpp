#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QDataStream>
#include "protocol.h"

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
    ui->leSend->setText("0001:0003:size:FFFF");
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

    QDataStream out(mSocket);

    out.writeRawData(buffer.toLocal8Bit(), buffer.size());
}

void Widget::buttonState(bool state)
{
    ui->leIP->setEnabled(state);
    ui->lePort->setEnabled(state);
    ui->btConnect->setEnabled(state);

    ui->btDisconnect->setEnabled(!state);
    ui->gbSend->setEnabled(!state);
}
