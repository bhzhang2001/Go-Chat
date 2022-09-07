//groupinfo.h
#ifndef GROUPINFO_H
#define GROUPINFO_H

#include "userinfo.h"

class GroupInfo:public UserInfo{
public:
    GroupInfo(quint32 id=0,QString name="",QString pwd="");
    GroupInfo(const UserInfo& Group);
};


#endif // GROUPINFO_H
