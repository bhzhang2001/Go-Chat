#ifndef MSG_H
#define MSG_H

#include <QObject>

#include <QDialog>
#include <QString>
#include <QTcpSocket>
#include <QHostAddress>
#include <QHostInfo>
#include <QList>
#include <cstring>
#include <string.h>
class msg
{
public:
    msg(){
        flag="";
        IP="";
        content="";
    }
    msg(QString _flag, QByteArray _content):flag(_flag),content(_content){
        QString hostname = QHostInfo::localHostName();
           QHostInfo hostinfo = QHostInfo::fromName(hostname);
           QString localip = "";
           QList<QHostAddress> addList = hostinfo.addresses();
           if(!addList.isEmpty())
               for(int i = 0;i<addList.size();i++)
               {
                   QHostAddress aHost = addList.at(i);
                   if(QAbstractSocket::IPv4Protocol == aHost.protocol())
                   {
                       localip = aHost.toString();
                       break;
                   }
               }
        //IP = localip.toUtf8();
         IP="192.168.199.129";
    }
    QByteArray toQByteArray();
    msg fromQByteArray(QByteArray);
    QString flag;
    QString IP;
    QByteArray content;
};

#endif // MSG_H
