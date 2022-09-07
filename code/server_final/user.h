#ifndef USER_H
#define USER_H

#include <QObject>

class User{
public:
    User();
    User(quint32 id, QString name, QString img);
    ~User();
    quint32 getId() const;
    QString getUsername() const;
    QString getImage() const;
    void Set(quint32 id=0,QString Username = "",QString Img = "");
private:
    quint32 id;
    QString username;
    QString img;
};
#endif // USER_H
