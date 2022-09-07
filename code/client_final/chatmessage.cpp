//chatmessage.cpp
#include "chatmessage.h"

#include <QDataStream>

//constructor
ChatMessage::ChatMessage(quint32 sender, quint32 receiver, const QString& content) :
    _sender(sender),_receiver(receiver), _content(content){}

//getter
quint32 ChatMessage::getSender() const{
    return _sender;
}

quint32 ChatMessage::getReceiver() const{
    return _receiver;
}

QString & ChatMessage::getContent(){
    return _content;
}

//trans
// | quint32 sender | quint32 receiver |  quint32 content size | content |
ChatMessage ChatMessage::fromQByteArray(QByteArray chat_msg){
    QDataStream in(chat_msg);
    in.setVersion(QDataStream::Qt_5_12);
    ChatMessage recvChatMsg(0,0,"");
    //sender and recceiver
    in >> recvChatMsg._sender;
    in >> recvChatMsg._receiver;
    quint32 tmpSize;
    //content
    in >> tmpSize;
    QByteArray tmpContentBlock;
    tmpContentBlock.resize(tmpSize);
    in >> tmpContentBlock;
    recvChatMsg._content = QString(tmpContentBlock);

    return recvChatMsg;
}

// | quint32 sender | quint32 receiver |  quint32 content size | content |
QByteArray ChatMessage::toQByteArray(){
    QByteArray block;

    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    out << quint32(_sender);
    out << quint32(_receiver);

    out.device()->seek(block.size());
    //content
    QByteArray tmp = _content.toUtf8();
    out << quint32(sizeof(tmp));
    out << tmp;

    return block;
}
