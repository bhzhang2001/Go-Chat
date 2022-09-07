#include "server.h"
#include "ui_server.h"
#include "database_all.h"
#include <QDateTime>
#include <QMessageBox>
#include "msg.h"

server::server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);
    server_ip = "127.0.0.1";
    server_port = "9999";

    udpSocket=new QUdpSocket(this);
    udpSocket->bind(QHostAddress::AnyIPv4,server_port.toUShort());

    connect(udpSocket, SIGNAL(readyRead()),
            this,SLOT(recvData()));
}

server::~server()
{
    delete ui;
}

void server::on_send_message_Btn_clicked()
{
    QString send_message = ui->send_message_Edit->text();

    QDateTime dateTime= QDateTime::currentDateTime(); // get time
    QString time = dateTime .toString("yyyy-MM-dd hh:mm:ss"); // normalization
    ui->text_display->append(time + "  server say :\n" + send_message + "\n");

    ui->send_message_Edit->clear();
    ui->send_message_Edit->setFocus();
}

void server::recvData()
{
    msg *rcv_msg=new msg();
    while(udpSocket->hasPendingDatagrams())  // distinguish whether having sended message or not
    {
        QByteArray flag_message;
        flag_message.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(flag_message.data(),flag_message.size());  // read sended message
        msg m_msg;
        m_msg=rcv_msg->fromQByteArray(flag_message);  // m_msg --> class msg's sended message

        DataDB* db= DataDB::GetInstance();  // create SQL single_instance
        //qDebug() << "receive success"<<m_msg.flag;

//***************************************VERY IMPORTANT****************************************************//
//*************************INFORMATION FROM CLIENT --> ENCODER AND DECODER*********************************//

        if(m_msg.flag == "0001"){  // regisiter --> return ID
            UserInfo user1;
            user1 = user1.fromQByteArray(m_msg.content);

            MaxID =db->selectMaxId()+1;  // update MaxID
            user1.setid(MaxID);
            db->registerUserInfo(user1);  // upadate sql write down user1's information
            msg *msg_maxid = new msg();
            msg_maxid->flag = "S0001";
            msg_maxid->content.setNum(MaxID);

            udpSocket->writeDatagram(msg_maxid->toQByteArray(), QHostAddress(m_msg.IP), 9997);  // send MaxID to client
            //qDebug()<<"write success";

        }else if(m_msg.flag == "0002"){  //login --> return flist & glist
            UserInfo user1;
            user1 = user1.fromQByteArray(m_msg.content);
            quint32 flag = db->checkUserByIdAndPwd(user1.getID(), user1.getPwd());  // check whether user correct or not

            msg *msg_login = new msg();
            msg_login->flag = "S0002";
            msg_login->content.setNum(flag);
            udpSocket->writeDatagram(msg_login->toQByteArray(), QHostAddress(m_msg.IP), 9998); // send flag to client

            if(flag == 1){  // login successfully --> 1.link ID and IP & 2.send friend list and group lsit
                struct hb_msg hb_msg1;
                hb_msg1.ID = user1.getID();
                hb_msg1.IP = m_msg.IP;
                qDebug()<<"hb_ID:"<<hb_msg1.ID<<"hb_IP:"<<hb_msg1.IP;
                ID_IP_list.append(hb_msg1);  // link ID and IP
                qDebug()<<"length"<<ID_IP_list.length();
                for(int i=0;i<ID_IP_list.length();i++){
                    qDebug()<<"ID_IP_list:"<<ID_IP_list[i].IP;
                }
                msg *msg_flist = new msg();
                msg_flist->flag = "S0003";
                QList<QByteArray> flist = db->selectAllFriendsUserInfo(user1.getID());  // get db's friendlist
                for(int i = 0; i < flist.length(); i++){
                    msg_flist->content.resize(sizeof(flist[i]));
                    msg_flist->content = flist[i];
                    udpSocket->writeDatagram(msg_flist->toQByteArray(), QHostAddress(m_msg.IP), 9996);
                }
                msg *msg_flist_end = new msg();
                msg_flist_end->flag = "S0013";  // ending of S0003
                msg_flist_end->content = NULL;
                udpSocket->writeDatagram(msg_flist_end->toQByteArray(), QHostAddress(m_msg.IP), 9996);
                qDebug()<<"send_S0013_success!";

                msg *msg_glist = new msg();
                msg_glist->flag = "S0004";
                QList<QByteArray> glist = db->selectAllGroupInfo(user1.getID());  // get db's grouplist
                for(int i = 0; i < glist.length(); i++){
                    msg_glist->content.resize(sizeof(glist[i]));
                    msg_glist->content = glist[i];
                    udpSocket->writeDatagram(msg_glist->toQByteArray(), QHostAddress(m_msg.IP), 9996);
                }
                msg *msg_glist_end = new msg();
                msg_glist_end->flag = "S0014";  // ending of S0004
                msg_glist_end->content = NULL;
                udpSocket->writeDatagram(msg_glist_end->toQByteArray(), QHostAddress(m_msg.IP), 9996);
                qDebug()<<"send0002 success";
            }

        }else if(m_msg.flag == "0003"){ // click friend --> return friend's IP
            msg *msg_ip = new msg();
            msg_ip->flag = "S0005";

            quint32 another_client_id = m_msg.content.toUInt(); // m_msg->content: QByreArray->quint32
            QString another_clinet_ip;
            for(int i = 0; i < ID_IP_list.length(); i++){  // find friend's IP from ID_IP_list
                if(ID_IP_list[i].ID == another_client_id){
                    another_clinet_ip = ID_IP_list[i].IP;
                }
            }
            msg_ip->IP = another_clinet_ip;  // friend's IP is saved in msg.IP not msg.content
            msg_ip->content = NULL;
            udpSocket->writeDatagram(msg_ip->toQByteArray(), QHostAddress(m_msg.IP), 9996);
            //qDebug()<<"m_msg.IP: "<<m_msg.IP;

        }else if(m_msg.flag=="0004"){//group ID ,并把创建者加入群聊
            GroupInfo group1;
            qDebug()<<"information: "<<m_msg.content;
            group1 = group1.fromQByteArray(m_msg.content);
            quint32 Gmax_id=db->selectMaxGroupId()+1;
            msg *msg_maxid = new msg();
            msg_maxid->flag = "S0009";
            group1.setid(Gmax_id);
            db->registerGroupInfo(group1);
            msg_maxid->content=group1.toQByteArray();

            int which=0;
            for(;which<this->ID_IP_list.length() ;which++){
                if(m_msg.IP==this->ID_IP_list[which].IP){
                    break;
               }
            }
            quint32 my_id=ID_IP_list[which].ID;
            db->addGroupship(my_id,Gmax_id);
            udpSocket->writeDatagram(msg_maxid->toQByteArray(), QHostAddress(m_msg.IP), 9996);

        }else if(m_msg.flag =="0005"){  // add friend --> return userinfo
            QByteArray data=m_msg.content;
            qint32 my_id,his_id;
            his_id = data.toInt();
            int which_flag = 0;
            int which = 0;
            for(;which<this->ID_IP_list.length() ;which++){
                qDebug()<<this->ID_IP_list[which].IP;
                qDebug()<<m_msg.IP;
                if(m_msg.IP==this->ID_IP_list[which].IP){
                    which_flag = 1;
                    break;
               }
            }
            if(which_flag == 0){
                QMessageBox::warning(this, tr("WARNING"), tr("ID_IP_list error"), QMessageBox::Yes);
            }
            my_id=ID_IP_list[which].ID;
            db->addFriendship(my_id,his_id);
            db->addFriendship(his_id,my_id);
            UserInfo new_friend = db->selectUserInfoById(his_id);

            msg *msg_friend_name=new msg();
            msg_friend_name->flag="S0007";
            msg_friend_name->content = new_friend.toQByteArray();

            udpSocket->writeDatagram(msg_friend_name->toQByteArray(), QHostAddress(m_msg.IP), 9996);
        }else if(m_msg.flag =="0006"){  // add ggroup --> return groupinfo
            QByteArray data=m_msg.content;

            qint32 my_id,group_id;
            group_id=data.toInt();
            int which=0;
            int which_flag = 0;
            for(;which<this->ID_IP_list.length() ;which++){
                if(m_msg.IP==this->ID_IP_list[which].IP){
                    which_flag = 1;
                    break;
               }
            }
            if(which_flag == 0){
                QMessageBox::warning(this, tr("WARNING"), tr("login information from client error"), QMessageBox::Yes);
            }
            my_id=ID_IP_list[which].ID;
            db->addGroupship(my_id,group_id);

            GroupInfo new_group=db->selectGroupInfoByID (group_id);

            msg *msg_group_name=new msg();
            msg_group_name->flag = "S0008";
            msg_group_name->content = new_group.toQByteArray();

            udpSocket->writeDatagram(msg_group_name->toQByteArray(), QHostAddress(m_msg.IP),9996 );
        }else if(m_msg.flag == "0007"){  // click group --> return bool
            msg *msg_g_mem_list = new msg();
            QList<quint32> g_mem_list = db->selectAllGroupMember(m_msg.content.toInt());

            int ID_check[ID_IP_list.length()];  // ID_check to memorize the user who in group
            memset(ID_check, 0, sizeof(ID_check));

            for(int i = 0; i < g_mem_list.length(); i++){
                for(int j = 0; j < ID_IP_list.length(); j++){
                    if(g_mem_list[i] == ID_IP_list[j].ID){
                        ID_check[j] = 1;
                        continue;
                    }
                }
            }
            msg_g_mem_list->flag = "S0006";
            for(int i = 0; i < ID_IP_list.length(); i++){
                if(ID_check[i] == 0){
                    msg_g_mem_list->content.setNum(0);
                }else if(ID_check[i] == 1){
                    msg_g_mem_list->content.setNum(1);
                }
                udpSocket->writeDatagram(msg_g_mem_list->toQByteArray(), QHostAddress(m_msg.IP), 9996);
            }
        }else{
            QMessageBox::warning(this, tr("WARNING"), tr("flag error"), QMessageBox::Yes);
        }
    }
}
