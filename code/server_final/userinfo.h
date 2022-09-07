#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>

class UserInfo
{

public:
    explicit UserInfo(quint32 id = 0x0000,
            QString name = "",
            QString pwd = ""
        );
    quint32 getID() const;
    QString getName() const;
    QString getPwd() const;

    UserInfo& operator = (const UserInfo &user){
        id=user.id;
        name=user.name;
        pwd=user.pwd;
        return *this;
    }
    void setid(quint32 id){
        this->id = id;
    }



protected:
    quint32 id;
    QString name;
    QString pwd;

public:

    static UserInfo fromQByteArray(QByteArray chat_msg);
    QByteArray toQByteArray();
};
#endif // USERINFO_H


