#include "udpconnect.h"
#include <QObject>
#include <QUdpSocket>

Udpconnect::Udpconnect()
{
    udpSocket = new QUdpSocket();
    this->ipStr="127.0.0.1";
    this->port=-1;
    this->recvPort=-1;

    udpSocket->bind(QHostAddress::AnyIPv4,recvPort);
    QObject::connect(udpSocket,SIGNAL(readyRead()),this,SLOT(recvData()));
}

void Udpconnect::recvData(){

        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        this->msg = datagram.data();
        qDebug()<<"udpconnect msg:"<<this->msg;

        //TODO


}

void Udpconnect::sendData(QString sendStr)
{
   udpSocket->writeDatagram(
                sendStr.toStdString().c_str(),
                QHostAddress(ipStr.toStdString().c_str()),
                port);
}
