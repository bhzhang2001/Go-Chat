#include "creategroupdialog.h"
#include "ui_creategroupdialog.h"
#include "msg.h"
#include "groupinfo.h"
CreategroupDialog::CreategroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreategroupDialog)
{
    ui->setupUi(this);
    setWindowTitle("Create new group");

    udpSocket = new QUdpSocket();
    this->ipStr="127.0.0.1";
    this->port=9999;
}

CreategroupDialog::~CreategroupDialog()
{
    delete ui;
}

void CreategroupDialog::on_createBtn_clicked()
{
    QString usr = ui->lineEdit->text();
        GroupInfo new_group(0,usr,"999888");
        msg msg_tosend("0004",new_group.toQByteArray());

    udpSocket->writeDatagram(msg_tosend.toQByteArray(), QHostAddress(this->ipStr.toStdString().c_str()),this->port);
    accept();
}

