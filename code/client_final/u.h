#ifndef UDPTHREAD_H
#define UDPTHREAD_H

#include <QThread>
#include <QUdpSocket>

#include "imageframehead.h"
//线程类；UdpThread
class UdpThread : public QThread
{
    Q_OBJECT
public:
  UdpThread(QObject *parent = 0);
    ~UdpThread();

protected:
 void run();

signals:
    void sigRecvOk(char *buf, int len);

public slots:
    void slotRecv();
private:
    QUdpSocket *m_udpSocket;
    char *m_buf;

};

#endif // UDPTHREAD_H
