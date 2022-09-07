//user.cpp
#include "user.h"
#include<qdebug.h>

//constructor
User::User()
{
    id = 0;
    username = "";
    img = "";
}

User::User(quint32 Id, QString name, QString Img)
{
    id = Id;
    username = name;
    img = Img;
}
//destructor
User::~User(){}
//getter
quint32 User::getId() const
{
    return id;
}

QString User::getUsername() const
{
    return username;
}

QString User::getImage() const
{
    return img;
}
//set
void User::Set(quint32 Id,QString Username,QString Img){
    id = Id; username = Username; img = Img;
}
