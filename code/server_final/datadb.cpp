#include "datadb.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QVariant>
#include <QtDebug>

//静态
DataDB* DataDB::db = NULL;
DataDB::DataDB() {
    sqldb = QSqlDatabase::addDatabase("QSQLITE");
    sqldb.setDatabaseName("data.db");
    if (!sqldb.open()) {
        QMessageBox::warning(NULL, "错误", "打开数据库时出现错误!", QMessageBox::Yes);
    }
    //下列是初始化建表
    QSqlQuery query;
    query.exec("CREATE TABLE UserInfo ("
        "Id INTEGER PRIMARY KEY, "
        "Username VARCHAR(40) NOT NULL, "
        "Pwd VARCHAR(40) NOT NULL)"
        );
    //好友关系表，单向好友
    query.exec("CREATE TABLE Friendship ("
        "MyId INTEGER, "
        "FriendId INTEGER, "
        "FOREIGN KEY (MyId) REFERENCES UserInfo(Id),"
        "FOREIGN KEY (FriendId) REFERENCES UserInfo(Id))");
    //离线消息列表
    query.exec("CREATE TABLE OfflineMsg ("
        "Sender INTEGER, "
        "Reciever INTEGER, "
        "Msg VARCHAR(512),"
        "FOREIGN KEY (Sender) REFERENCES UserInfo(Id),"
        "FOREIGN KEY (Reciever) REFERENCES UserInfo(Id))");
    query.exec("CREATE TABLE Msg ("
        "Sender INTEGER,"
        "Reciever INTEGER,"
        "Msg VARCHAR(512),"
        "FOREIGN KEY (Sender) REFERENCES UserInfo(Id),"
        "FOREIGN KEY (Reciever) REFERENCES UserInfo(Id))");

    //群聊ID与用户ID区分开
    query.exec("CREATE TABLE GroupInfo ("
          "ID INTEGER PRIMARY KEY,"
          "GroupName VARCHAR(40))"
          );
    query.exec("CREATE TABLE Groupship("
              "UserID INTEGER,"
              "GroupID INTEGER,"
              "FOREIGN KEY (UserID) REFERENCES UserInfo(Id),"
              "FOREIGN KEY (GroupID) REFERENCES GroupInfo(ID))");
    //建表完成

}
DataDB::~DataDB() {
}
DataDB* DataDB::GetInstance() {
    if (db == NULL) {
        db = new DataDB;
    }
    return db;
}
//查询所有好友,一个包含User Info的QList
QList<QByteArray> DataDB::selectAllFriendsUserInfo(quint32 UserId) {
    QSqlQuery query;
    query.prepare("select Id,Username from UserInfo where Id in (select FriendId from Friendship where MyId =:UserId)");
    query.bindValue(":UserId", QVariant(UserId));
    query.exec();
    QList<QByteArray> ListUserInfo;
    ListUserInfo.clear();
    while (query.next()) {
        ListUserInfo.append(UserInfo(query.value("Id").toInt(), query.value("Username").toString(),"").toQByteArray());
    }
    return ListUserInfo;
}



//注册信息
void DataDB::registerUserInfo(const UserInfo& user) {
    QSqlQuery query;
    query.prepare("insert into UserInfo values(:Id,:Username,:pwd)");
    query.bindValue(":Id", user.getID());
    query.bindValue(":Username", user.getName());
    query.bindValue(":pwd", user.getPwd());
    query.exec();
}

//登录验证
bool DataDB::checkUserByIdAndPwd(const quint32 Id, const QString pwd) {
    QSqlQuery query;
    query.prepare("select * from UserInfo where Id =:Id and pwd =:pwd");
    query.bindValue(":Id", QVariant(Id));
    query.bindValue(":pwd", QVariant(pwd));
    query.exec();
    bool flag = query.next();
    return flag;
}

//获得不含密码的基本信息
UserInfo DataDB::selectUserInfoById(const quint32 id){
    QSqlQuery query;
    query.prepare("select * from UserInfo where Id =:Id");
    query.bindValue(":Id", QVariant(id));
    query.exec();
    UserInfo tmp;
    while(query.next()){
        tmp = UserInfo(query.value("Id").toUInt(),
                       query.value("Username").toString(),
                       ""
                       );
    }
    return tmp;
}

//添加好友
void DataDB::addFriendship(quint32 Id1, quint32 Id2) {
    QSqlQuery query;
    query.prepare("insert into Friendship values(:Id1,:Id2)");
    query.bindValue(":Id1", QVariant(Id1));
    query.bindValue(":Id2", QVariant(Id2));
    query.exec();
}
//删除好友
void DataDB::deleteFriendship(quint32 Id1, quint32 Id2) {
    QSqlQuery query;
    query.prepare("delete from Friendship where MyId =:Id1 and FriendId =:Id2");
    query.bindValue(":Id1", QVariant(Id1));
    query.bindValue(":Id2", QVariant(Id2));
    query.exec();
}
void DataDB::addMsg(quint32 Id1, quint32 Id2, QString Msg) {
    QSqlQuery query;
    query.prepare("insert into Msg values(:Id1,:Id2,:Msg,:Date)");
    query.bindValue(":Id1", QVariant(Id1));
    query.bindValue(":Id2", QVariant(Id2));
    query.bindValue(":Msg", Msg);
    query.exec();
}

quint32 DataDB::selectMaxId() {
    QSqlQuery query;
    query.exec("select MAX(Id) as Id from UserInfo");
    quint32 maxId = quint32(1000);
    while (query.next()) {
        maxId = query.value("Id").toUInt();
    }
    if(maxId>=quint32(1000)) return maxId;
    else return quint32(1000);
}
void DataDB::addGroupship(quint32 UserID,quint32 GroupID){
    QSqlQuery query;
    query.prepare("insert into Groupship values(:UserID,:GroupID)");
    query.bindValue(":UserID", QVariant(UserID));
    query.bindValue(":GroupID", QVariant(GroupID));
    query.exec();
}
void DataDB::registerGroupInfo(const GroupInfo& GroupInfo){
    QSqlQuery query;
    query.prepare("insert into GroupInfo values(:Id,:Username)");
    query.bindValue(":Id", QVariant(GroupInfo.getID()));
    query.bindValue(":Username", QVariant(GroupInfo.getName()));
    query.exec();
}
void DataDB::addOfflineMsg(ChatMessage &msg){
    QSqlQuery query;
    query.prepare("insert into OfflineMsg values(:Sender,:Reciever,:Msg,:DT)");
    query.bindValue(":Sender",QVariant(msg.getSender()));
    query.bindValue(":Reciever",QVariant(msg.getReceiver()));
    query.bindValue(":Msg",QVariant(msg.getContent()));
    query.exec();
}
QList<ChatMessage> DataDB::getOfflineMsg(quint32 ID){
    QList<ChatMessage> msg;
    msg.clear();
    QSqlQuery query;
    query.prepare("select * from OfflineMsg where Reciever = :ID");
    query.bindValue(":ID",QVariant(ID));
    query.exec();
    while (query.next()) {
        ChatMessage nowmsg(query.value("Sender").toUInt(),query.value("Reciever").toUInt()
                           ,query.value("Msg").toString());
        msg.append(nowmsg);
    }
    return msg;
}
void DataDB::dropOfflineMsg(quint32 ID){
    QSqlQuery query;
    query.prepare("delete from OfflineMsg where Reciever = :ID");
    query.bindValue(":ID",QVariant(ID));
    query.exec();
}

quint32 DataDB::selectMaxGroupId() {//群聊7位数
    QSqlQuery query;
    query.exec("select MAX(Id) as Id from GroupInfo");
    quint32 maxId = quint32(1000000);
    while (query.next()) {
        maxId = query.value("Id").toUInt();
    }
    if(maxId>=quint32(1000000)) return maxId;
    else return quint32(1000000);
}

void DataDB::updUsername(quint32 ID, QString Username){
    QSqlQuery query;
    query.prepare("UPDATE UserInfo SET Username = :Username WHERE ID = :ID");
    query.bindValue(":ID",QVariant(ID));
    query.bindValue(":Username",QVariant(Username));
    query.exec();
}


GroupInfo DataDB::selectGroupInfoByID(quint32 ID){
    QSqlQuery query;
    query.prepare("select * from GroupInfo where ID = :ID");
    query.bindValue(":ID",QVariant(ID));
    query.exec();
    GroupInfo tmp;
    while(query.next()){
        tmp = GroupInfo(query.value("Id").toUInt(),
                       query.value("GroupName").toString(),
                       ""
                       );
    }
    return tmp;
}
bool DataDB::friendshipExist(quint32 Id1,quint32 Id2){
    QSqlQuery query;
    query.prepare("select * from Friendship where MyId =:Id1 and FriendId =:Id2");
    query.bindValue(":Id1", QVariant(Id1));
    query.bindValue(":Id2", QVariant(Id2));
    query.exec();
    bool flag = query.next();
    return flag;
}
bool DataDB::groupshipExist(quint32 Id1, quint32 Id2){
    QSqlQuery query;
    query.prepare("select * from Groupship where UserId =:Id1 and GroupId =:Id2");
    query.bindValue(":Id1", QVariant(Id1));
    query.bindValue(":Id2", QVariant(Id2));
    query.exec();
    bool flag = query.next();
    return flag;
}
QList<QByteArray> DataDB::selectAllGroupInfo(quint32 ID){
    QSqlQuery query;
    query.prepare("select Id,GroupName from GroupInfo where Id in (select GroupID from Groupship where UserId =:UserId)");
    query.bindValue(":UserId", QVariant(ID));
    query.exec();
    QList<QByteArray> ListGroupInfo;
    ListGroupInfo.clear();
    while (query.next()) {
        ListGroupInfo.append(UserInfo(query.value("Id").toInt(), query.value("GroupName").toString(),"").toQByteArray());
    }
    return ListGroupInfo;
}
QList<quint32> DataDB::selectAllGroupMember(quint32 ID){
    QSqlQuery query;
    query.prepare("select UserId from Groupship where GroupID = :ID");
    query.bindValue(":ID",QVariant(ID));
    QList<quint32> ListGroupMember;
    ListGroupMember.clear();
    query.exec();
    while(query.next()){
        ListGroupMember.append(query.value("UserId").toUInt());
    }

    return ListGroupMember;
}
