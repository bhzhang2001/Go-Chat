//groupinfo.cpp
#include "groupinfo.h"

#include<QDataStream>

//constructor
GroupInfo::GroupInfo(quint32 id,
            QString name,
            QString pwd
        ):UserInfo(id,name,pwd){}
//copy constructor
GroupInfo::GroupInfo(const UserInfo &Group){
    this->id = Group.getID(); this->name = Group.getName();
    this->pwd = Group.getPwd();
}

