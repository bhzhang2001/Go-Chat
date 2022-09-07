#include "filepkg.h"

#include <QDataStream>

FileMeta::FileMeta(UserInfo user, QString original_name)
    :original_name(original_name)
{
    int dot_index = original_name.lastIndexOf("/")+1;
    unique_name = original_name.mid(dot_index);//扩展名
}

QString FileMeta::toQString(){
    return QString(unique_name.toUtf8().toBase64());
}

//先给文件信息储存成一个单位
FileMeta FileMeta::fromQByteArray(QByteArray msg_content){

    QDataStream in(msg_content);
    in.setVersion(QDataStream::Qt_5_12);
    quint32 size;
    in >> size;
    QByteArray tmpMeta;
    tmpMeta.resize(size);
    in >> tmpMeta;
    FileMeta tmp;
    tmp.unique_name = QString(tmpMeta);
    tmp.original_name = "";
    return tmp;
}

//|quint32 size_fileinfo | QString fileinfo | quint32 size_content | QByteArray content | quint32 sender | quint32 receiver
QByteArray FileMeta::toQByteArray(){
    QByteArray block;

    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);

    int size_index = block.size();
    out << quint32(0);
    out << unique_name.toUtf8();
    out.device()->seek(size_index);
    out << (quint32)(block.size() - size_index - sizeof(quint32));
    out.device()->seek(block.size());
    return block;
}
