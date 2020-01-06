#ifndef ROVERCLIENTWINDOW_H
#define ROVERCLIENTWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class RoverClientWindow; }
QT_END_NAMESPACE

class RoverClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    RoverClientWindow(QWidget *parent = nullptr);
    ~RoverClientWindow();

private slots:
    void on_btConnect_clicked();
    void on_btDisconnect_clicked();
    void on_btUp_clicked();
    void on_btLeft_clicked();
    void on_btCenter_clicked();
    void on_btRight_clicked();
    void on_btDown_clicked();

private:
    Ui::RoverClientWindow *ui;
};
#endif // ROVERCLIENTWINDOW_H
