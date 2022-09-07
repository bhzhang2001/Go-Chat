#ifndef UDPCONNECT_H
#define UDPCONNECT_H

#include <QObject>
#include <QUdpSocket>
class Udpconnect:QObject
{
    Q_OBJECT
public:
    Udpconnect();
    int port;
    int recvPort;
    QString ipStr;
    QUdpSocket *udpSocket;
    QString msg;
public slots:
    void recvData();
    void sendData(QString sendStr);
};

#endif // UDPCONNECT_H
