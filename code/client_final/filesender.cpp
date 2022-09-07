#include "filesender.h"
#include <QTime>
#include <QDesktopWidget>
#include<QDebug>
#include<QFileInfo>//获取文件信息
#include <QCoreApplication>

filesender::filesender(QWidget *parent) : QMainWindow(parent)
{
    m_udpSocket = new QUdpSocket(this);
    sendsize=0;
    m_udpSocket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption,8*1024*1024);
    m_udpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,500000);


}
filesender::~filesender()
{

}
void filesender::comeOnfile(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))return;
    //包1024=包头28(7个整形)+数据（996）
    char *m_sendBuf = new char[1024];
    //size:文件大小
    int size = file.size();
    qDebug()<<"文件大小"<<size;
    //num:数据包数量
    int num = 0;
    //count：数据包序号
    int count = 0;
    //最后一个包的大小
    int endSize = size%996;
    if (endSize == 0) {
        num = size/996;
    }
    else {
        num = size/996+1;//num:包的数量
    }
    qint64 len = 0;//记录发送数据

//         //一、发送文件信息
//         //1.获取文件路径
//         QString filepath = "D:/3.ts";
//         //2.获取文件信息
//         QFileInfo info(filepath);
//         fileName = info.fileName();

//         fileSize = info.size();
//         //3.设置文件名
//         file.setFileName(filepath);

    //二、发送数据
    //1、组包：28字节包头+996字节数据
    //2、发送：广播到port：65522
    while (count < num) {
        //初始化：将m_sendBuf的前1024个字符置0
        memset(m_sendBuf, 0, 1024);
        //包的结构体
        ImageFrameHead mes;
        //功能码24
        mes.funCode = 24;
        //包头长度=4*7=28
        mes.uTransFrameHdrSize = sizeof(ImageFrameHead);
        if ((count+1) != num) {
            //数据长度
            mes.uTransFrameSize = 996;

        }
        else {
            //最后一个包的长度
            mes.uTransFrameSize = endSize;
        }
        //qDebug()<<size;
        //数据帧总大小=文件大小
        mes.uDataFrameSize = size;
        //包个数
        mes.uDataFrameTotal = num;
        //包序号
        mes.uDataFrameCurr = count+1;
        //数据996的整数倍
        mes.uDataInFrameOffset = count*(1024 - sizeof(ImageFrameHead));
        qDebug()<<"uDataInFrameOffset"<<mes.uDataInFrameOffset;
        //读取文件，参数1：存储位置——指针，参数2：maxsize
        //参数1：意思是指向m_sendBuf向后移28位的位置，将996位的数据存在这里
        //从文件里读取996字节数据写入m_sendBuf数据包里数据的位置
        // data:file(996字节)->m_sendBuf(29字节起-1024字节)
        len = file.read(m_sendBuf+sizeof(ImageFrameHead), 1024-sizeof(ImageFrameHead));
        sendsize+=len;
        qDebug()<<"已发送文件大小"<<sendsize;
        //从mes里复制28个字节给m_sendBuf，mes:包头结构体。
        //构建包：m_sendBuf的前28字节写包的信息。
        memcpy(m_sendBuf, (char *)&mes, sizeof(ImageFrameHead));
        //发数据：m_sendBuf即包。
        //参数2：包的长度=表头+数据=1024，最后一个包不一定
        m_udpSocket->writeDatagram(m_sendBuf, mes.uTransFrameSize+mes.uTransFrameHdrSize, QHostAddress("224.0.0.10"), 65522);
        //currentTime:返回当前时间
        //addMSecs:返回QTime对象dieTime，其时间比当前时间延后1ms
        //dieTime = 当前时间 + 1ms
        QTime dieTime = QTime::currentTime().addMSecs(1);
        //防止卡死，隔一段时间检查有没有未完成操作
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        count++;
    }
    qDebug()<<"数据发送完成";
    file.close();
    QTime dieTime = QTime::currentTime().addMSecs(10);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    //qDebug("ok");
}

