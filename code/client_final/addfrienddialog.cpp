#include "addfrienddialog.h"
#include "ui_addfrienddialog.h"
#include <QDebug>
#include "msg.h"
#include "userinfo.h"
#include "groupinfo.h"
AddfriendDialog::AddfriendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddfriendDialog)
{
    ui->setupUi(this);
    setWindowTitle("Add new friend");

    udpSocket = new QUdpSocket();
    this->ipStr="127.0.0.1";
    this->port=9999;
}

AddfriendDialog::~AddfriendDialog()
{
    delete ui;
}

void AddfriendDialog::on_pushButton_clicked()
{
    QString qid = ui->lineEdit->text();

    QByteArray id_tmp;
    quint32 id=qid.toUInt();
    id_tmp.setNum(id);
    msg msg_tosend("0005",id_tmp);
    //send
    QUdpSocket *udpSocket=new QUdpSocket;
    udpSocket->writeDatagram(msg_tosend.toQByteArray(), QHostAddress(ipStr.toStdString().c_str()),port);
    accept();
}

void AddfriendDialog::sendData(QString sendStr)
{
   udpSocket->writeDatagram(
                sendStr.toStdString().c_str(),
                QHostAddress(ipStr.toStdString().c_str()),
                port);
}

