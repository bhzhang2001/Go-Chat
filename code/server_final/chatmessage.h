//chatmessage.h
#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QObject>

class ChatMessage
{

public:
    ChatMessage(){}
    ChatMessage(quint32 sender, quint32 receiver, const QString& content);
    ChatMessage& operator = (const ChatMessage &msg){
        _sender=msg._sender;
        _receiver=msg._receiver;
        _content=msg._content;
        return *this;
    }
    //getters
    quint32 getSender() const;
    quint32 getReceiver() const;
    QString & getContent();
    friend class DataDB;


private:
    quint32 _sender;
    quint32 _receiver;
    QString _content;

public:

    //转换函数, 便于丢给Msg后再发送
    //对于ChatMessage的QByteArray编码方式
    // | quint32 sender | quint32 receiver |  quint32 content size | content |

    static ChatMessage fromQByteArray(QByteArray chat_msg);
    QByteArray toQByteArray();

};

#endif // CHATMESSAGE_H

