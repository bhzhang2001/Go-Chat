#include "addgroupdialog.h"
#include "ui_addgroupdialog.h"
#include "msg.h"
#include <QDebug>
AddgroupDialog::AddgroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddgroupDialog)
{
    ui->setupUi(this);
    setWindowTitle("Add new group");

    udpSocket = new QUdpSocket();
    this->ipStr="127.0.0.1";
    this->port=9999;
}

AddgroupDialog::~AddgroupDialog()
{
    delete ui;
}

void AddgroupDialog::on_addBtn_clicked()
{
    QString qid = ui->lineEdit->text();
    QByteArray id_tmp;
    quint32 id=qid.toUInt();
    id_tmp.setNum(id);
    msg msg_tosend("0006",id_tmp);
    //send
    QUdpSocket *udpSocket=new QUdpSocket;
    udpSocket->writeDatagram(msg_tosend.toQByteArray(), QHostAddress(ipStr.toStdString().c_str()),port);
    //TODO
    qDebug()<<"addgroup lineEdit:"<<id;
    accept();
}


void AddgroupDialog::sendData(QString sendStr)
{
   udpSocket->writeDatagram(
                sendStr.toStdString().c_str(),
                QHostAddress(ipStr.toStdString().c_str()),
                port);
}

