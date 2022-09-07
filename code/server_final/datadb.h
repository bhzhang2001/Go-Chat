#ifndef DATADB_H
#define DATADB_H

#include "userinfo.h"
#include "chatmessage.h"
#include "groupinfo.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class DataDB
{
public:
    DataDB();
    ~DataDB();

    //检查是否存在用户/用户信息是否正确
    bool checkUserByIdAndPwd(const quint32 id, const QString pwd);//存在返回true，不存在返回false

    //根据id返回用户信息
    UserInfo selectUserInfoById(const quint32 id);

    //查询当前全部用户，获取所有好友信息
    QList<QByteArray> selectAllFriendsUserInfo(quint32 UserId);

    //注册
    void registerUserInfo(const UserInfo& user);//一个用户结构

    //修改用户名和头像
    void updUsername(quint32 ID,QString Username);
//    void updAvatar(quint32 ID,quint32 Avatar);

    //添加好友
    void addFriendship(quint32 Id1,quint32 Id2);//两个好友的Id

    //删除好友
    void deleteFriendship(quint32 Id1,quint32 Id2);//两个好友的Id

    //判断好友关系是否存在
    bool friendshipExist(quint32 Id1,quint32 Id2);

    //添加聊天信息:can用于消息历史记录
    void addMsg(quint32 Id1,quint32 Id2,QString Msg);//发送者的Id，接收者的Id，聊天信息

    //获取最大的Id值:用于注册时+1
    quint32 selectMaxId();

    //处理离线消息：
    //添加离线消息
    void addOfflineMsg(ChatMessage &msg);
    //获取离线消息
    QList<ChatMessage> getOfflineMsg(quint32 ID);
    //删除离线消息
    void dropOfflineMsg(quint32 ID);

    //群组操作
    //添加群聊关系
    void addGroupship(quint32 UserID,quint32 GroupID);

    //添加群聊信息
    void registerGroupInfo(const GroupInfo& Group);
    //as same as user
    quint32 selectMaxGroupId();
    GroupInfo selectGroupInfoByID(quint32);
    // Id1用户ID,Id2群聊ID,返回Id1是否在Id2中
    bool groupshipExist(quint32 Id1, quint32 Id2);
    //获取用户加入的所有群组信息
    QList<QByteArray> selectAllGroupInfo(quint32 ID);
    //选取所有成员
    QList<quint32> selectAllGroupMember(quint32 ID);

    //单例
    static DataDB* GetInstance();

private:
    QSqlDatabase sqldb;
    static DataDB* db;//单例实现
};


#endif // DATADB_H
