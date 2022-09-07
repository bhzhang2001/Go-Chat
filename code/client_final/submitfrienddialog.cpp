#include "submitfrienddialog.h"
#include "ui_submitfrienddialog.h"
#include <QString>
SubmitfriendDialog::SubmitfriendDialog(QWidget *parent,QString msg) :
    QDialog(parent),
    ui(new Ui::SubmitfriendDialog)
{
    ui->setupUi(this);
    setWindowTitle("submit new friend");
    ui->textEdit->append(msg+" send you a request");
    //udpconnect
    udpSocket = new QUdpSocket();
    this->ipStr="127.0.0.1";
    this->port=9999;
}

SubmitfriendDialog::~SubmitfriendDialog()
{
    delete ui;
}

void SubmitfriendDialog::on_acceptBtn_clicked()
{
    sendData("1");
    accept();
}


void SubmitfriendDialog::on_rejectBtn_clicked()
{
    sendData("0");
    accept();
}

void SubmitfriendDialog::sendData(QString sendStr)
{
   udpSocket->writeDatagram(
                sendStr.toStdString().c_str(),
                QHostAddress(ipStr.toStdString().c_str()),
                port);
}

