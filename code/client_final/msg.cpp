#include "msg.h"

QByteArray msg::toQByteArray(){
    QByteArray block;

    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);

    int size_index = block.size();
    out << quint32(0);
    out << flag.toUtf8();
    out.device()->seek(size_index);
    out << (quint32)(block.size() - size_index - sizeof(quint32));
    out.device()->seek(block.size());

    size_index = block.size();
    out << quint32(0);
    out << IP.toUtf8();
    out.device()->seek(size_index);
    out << (quint32)(block.size() - size_index - sizeof(quint32));
    out.device()->seek(block.size());


    size_index = block.size();
    out << quint32(0);
    out << content;
    out.device()->seek(size_index);
    out << (quint32)(block.size() - size_index - sizeof(quint32));
    out.device()->seek(block.size());
    return block;
}
//|size_flag | flag | size_IP | IP | size_content | content
msg msg::fromQByteArray(QByteArray data){

    QDataStream in(data);
    in.setVersion(QDataStream::Qt_5_12);

    msg tmp;
    quint32 size;
    in >> size;
    QByteArray tmpName;
    tmpName.resize(size);
    in >> tmpName;
    tmp.flag = QString(tmpName);

    in >> size;
    QByteArray tmpip;
    tmpip.resize(size);
    in >> tmpip;
    tmp.IP = QString(tmpip);

    in >> size;
    QByteArray content_tmp;
    content_tmp.resize(size);
    in>>content_tmp;
    tmp.content=content_tmp;
    return tmp;
}
