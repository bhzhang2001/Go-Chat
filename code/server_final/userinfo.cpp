#include "userinfo.h"

#include<QDataStream>

//constructor
UserInfo::UserInfo(quint32 id,
            QString name,
            QString pwd
        ): id(id), name(name), pwd(pwd){}

// | quint32 id | quint32 name size | name | quint32 pwd size | pwd |

//getter
quint32 UserInfo::getID() const{
    return id;
}

QString UserInfo::getName() const{
    return name;
}

QString UserInfo::getPwd() const{
    return pwd;
}


//trans
UserInfo UserInfo::fromQByteArray(QByteArray chat_msg){

    QDataStream in(chat_msg);
    in.setVersion(QDataStream::Qt_5_12);

    UserInfo tmp;
    //read id
    in >> tmp.id;

    //read name
    quint32 size;
    in >> size;
    QByteArray tmpName;
    tmpName.resize(size);
    in >> tmpName;
    tmp.name = QString(tmpName);

    //read pwd
    in >> size;
    QByteArray tmpPwd;
    tmpPwd.resize(size);
    in >> tmpPwd;
    tmp.pwd = QString(tmpPwd);


    return tmp;
}

// | quint32 id | quint32 name size | name | quint32 pwd size | pwd |

QByteArray UserInfo::toQByteArray(){

    QByteArray block;

    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);

    //输出id
    out << quint32(id);

    //输出name
    out << quint32(0);//给name size的front 0
    out << name.toUtf8();
    out.device()->seek(4);//size pos to size
    out << (quint32)(block.size()-sizeof(quint32)*2);//two int
    out.device()->seek(block.size());

    //输出pwd
    int size_index = block.size();
    out << quint32(0);//as same
    out << pwd.toUtf8();

    out.device()->seek(size_index);
    out << (quint32)(block.size() - size_index - sizeof(quint32));
    out.device()->seek(block.size());


    return block;
}

