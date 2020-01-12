#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QDataStream>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void buttonState(bool state);

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

private:
    Ui::Widget *ui;
    QTcpSocket *mSocket;
    bool mSocketReady;
    QDataStream out;
};
#endif // WIDGET_H
