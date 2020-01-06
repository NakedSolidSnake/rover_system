#include "roverclientwindow.h"
#include "ui_roverclientwindow.h"

RoverClientWindow::RoverClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RoverClientWindow)
{
    ui->setupUi(this);
}

RoverClientWindow::~RoverClientWindow()
{
    delete ui;
}

void RoverClientWindow::on_btConnect_clicked()
{

}

void RoverClientWindow::on_btDisconnect_clicked()
{

}

void RoverClientWindow::on_btUp_clicked()
{

}

void RoverClientWindow::on_btLeft_clicked()
{

}

void RoverClientWindow::on_btCenter_clicked()
{

}

void RoverClientWindow::on_btRight_clicked()
{

}

void RoverClientWindow::on_btDown_clicked()
{

}
