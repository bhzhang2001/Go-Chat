#include "logindialog.h"
#include "ui_logindialog.h"
#include "registerdialog.h"
#include <QDebug>
#include "mainwindow.h"
#include <QMessageBox>
#include "msg.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{

    ui->setupUi(this);
    setWindowTitle("Go Chat");

    //udp connect
    udpSocket = new QUdpSocket();
    this->ipStr="127.0.0.1";
    this->port=9999;
    this->recvPort=9998;
    udpSocket->bind(QHostAddress::AnyIPv4,recvPort);
    QObject::connect(udpSocket,SIGNAL(readyRead()),this,SLOT(recvData()));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginBtn_clicked()
{
    QString pwd = ui->pwdLineEdit->text();
    QString usr = ui->userLineEdit->text();

    //login ask
    quint32 id=usr.toUInt();

    Myid=id;

    UserInfo user(id,"",pwd);
    //send
    msg msg_tosend("0002",user.toQByteArray());
    udpSocket->writeDatagram(msg_tosend.toQByteArray(), QHostAddress(ipStr.toStdString().c_str()),port);
    //get
}

void LoginDialog::on_registerBtn_clicked()
{
    RegisterDialog rgdl;
    rgdl.exec();
}

void LoginDialog::recvData(){
    bool fl=0;
    while(udpSocket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        msg msg_get;
        msg_get=msg_get.fromQByteArray(datagram);
        if(msg_get.flag=="S0002"){
            qDebug()<<"login";
            fl=msg_get.content.toUInt();
        }
        //check if exist
            //get friendshipinfo and groupinfo there?
            //get

        else{
            qDebug()<<"error";
        }
    }
    if(fl){
         accept();
    }
    else{
        QMessageBox::warning(this,tr("wrong"),
                             tr("id or password error"),
                             QMessageBox::Yes);
    }
}

void LoginDialog::sendData(QString sendStr)
{
   udpSocket->writeDatagram(
                sendStr.toStdString().c_str(),
                QHostAddress(ipStr.toStdString().c_str()),
                port);
}
