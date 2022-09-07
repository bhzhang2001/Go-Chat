#include "udp.h"
#include <QObject>
#include <QUdpSocket>
Udp::Udp()
{
    udpSocket = new QUdpSocket();
    this->ipStr="127.0.0.1";
    this->port=9998;
    this->recvPort=9999;

    udpSocket->bind(QHostAddress::AnyIPv4,recvPort);
    QObject::connect(udpSocket,SIGNAL(readyRead()),this,SLOT(recvData()));
}

void Udp::recvData(){
    while(udpSocket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QString msg = datagram.data();
    }
}
