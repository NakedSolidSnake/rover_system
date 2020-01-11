#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>

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



private:
    Ui::Widget *ui;
    QTcpSocket *mSocket;
    bool mSocketReady;
};
#endif // WIDGET_H
