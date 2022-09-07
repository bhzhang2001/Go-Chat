#include "u.h"
//1、构造
UdpThread::UdpThread(QObject *parent) :
    QThread(parent)
{
    m_buf = new char[100*1024*1024];
    //初始化：将m_buf的前1024个字符置0
    memset(m_buf, 0, 1024);

}
//2、析构
UdpThread::~UdpThread()
{
    m_udpSocket->close();
    delete m_buf;
}
//3、run
void UdpThread::run()
{
    m_udpSocket = new QUdpSocket(this);
    m_udpSocket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption,8*1024*1024);
    m_udpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,500000);
    m_udpSocket->bind(QHostAddress::AnyIPv4,65522);
    m_udpSocket->joinMulticastGroup(QHostAddress("224.0.0.10"));

    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(slotRecv()),Qt::DirectConnection);
    //qDebug()<< m_udpSocket->bind(QHostAddress::Any, 65522);
    exec();

}

void UdpThread::slotRecv()
{
    //1024_+28=1052
    char *recvBuf = new char[1052];
    memset(recvBuf, 0, 1052);
    //qDebug("start");
    while(m_udpSocket->hasPendingDatagrams()) {
        memset(recvBuf, 0, 1052);
        int size = m_udpSocket->pendingDatagramSize();
        //qDebug()<<"size"<<size;
        //1、包->recvBuf
        m_udpSocket->readDatagram(recvBuf, size);
        //2、包：recvBuf->mes
        ImageFrameHead *mes = (ImageFrameHead *)recvBuf;

        //qDebug()<<mes->uDataFrameCurr<<mes->uDataFrameTotal;
        if (mes->funCode == 24) {
            //3.将recvBuf地址中第28字符开始复制996个字节到m_buf中0、996、 2*996、3*996的位置
            memcpy(m_buf + mes->uDataInFrameOffset, (recvBuf+ sizeof(ImageFrameHead)), mes->uTransFrameSize);
            //4、包序号=包总数，即接收完毕
            if (mes->uDataFrameCurr == mes->uDataFrameTotal) {
                qDebug("write over");
                //qDebug()<<"m_buf"<<m_buf<<"  mes->uDataFrameSize"<<mes->uDataFrameSize<<

                emit sigRecvOk(m_buf, mes->uDataFrameSize);
            }
        }

    }
    delete recvBuf;
    this->exit();
    this->wait();
}
