#include "registerdialog.h"
#include "ui_registerdialog.h"
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include "userinfo.h"
#include "msg.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{

    ui->setupUi(this);
    setWindowTitle("register");

    //udp connect
    udpSocket = new QUdpSocket();
    this->ipStr="127.0.0.1";
    this->port=9999;
    this->recvPort=9997;

    udpSocket->bind(QHostAddress::AnyIPv4,recvPort);
    QObject::connect(udpSocket,SIGNAL(readyRead()),this,SLOT(recvData()));
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_registerBtn_clicked()
{
    QString usr = ui->userLineEdit->text();
    QString pwd = ui->pwdLineEdit->text();
    QString rpwd = ui->verifyLineEdit->text();

    if(check_usrname(usr)==false){
        QMessageBox::warning(this,tr("wrong"),
                             tr("username is illegal"),
                             QMessageBox::Yes);
    }
    else if(check_pwd(pwd)==false){
        QMessageBox::warning(this,tr("wrong"),
                             tr("password is illegal"),
                             QMessageBox::Yes);
    }
    else if(QString::compare(rpwd,pwd,Qt::CaseInsensitive) != 0){

        QMessageBox::warning(this,tr("wrong"),
                             tr("confirm password incorrect"),
                             QMessageBox::Yes);
    }
    else{
        //TODO
        qDebug()<<"register userEdit:"<<usr;
        qDebug()<<"register pwdEdit:"<<pwd;

        UserInfo new_user(0,usr,pwd);
        msg msg_tosend("0001",new_user.toQByteArray());

        udpSocket->writeDatagram(msg_tosend.toQByteArray(), QHostAddress(this->ipStr.toStdString().c_str()),this->port);
    }
}

bool RegisterDialog::check_usrname(QString usr){
    QRegExp regExp("[a-zA-Z0-9]{8,20}");
    return regExp.exactMatch(usr);
}

bool RegisterDialog::check_pwd(QString pwd){
    QRegExp regExp("[a-zA-Z0-9]{5,20}");
    return regExp.exactMatch(pwd);
}

void RegisterDialog::recvData(){

        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        msg msg_get;
        msg_get=msg_get.fromQByteArray(datagram);
        qDebug()<<msg_get.IP;
        quint32 id=msg_get.content.toUInt();
        qDebug()<<datagram;
        qDebug()<<msg_get.flag;
        qDebug()<<id;
        QMessageBox::information(this,tr("success!"),
                             tr("your id is%5").arg(id),
                             QMessageBox::Yes);
        accept();

}
