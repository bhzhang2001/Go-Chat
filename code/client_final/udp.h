#ifndef UDP_H
#define UDP_H

#include <QString>
#include <QUdpSocket>
#include <QObject>

class Udp
{
public:
    Udp();
    int port;
    int recvPort;
    QString ipStr;
    QUdpSocket *udpSocket;

    void recvData();
};

#endif // UDP_H
