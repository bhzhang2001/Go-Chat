#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "addfrienddialog.h"
#include "addgroupdialog.h"
#include <QtDebug>
#include <QToolButton>
#include "userinfo.h"
#include "submitfrienddialog.h"
#include "groupinfo.h"
#include "msg.h"
#include "chatmessage.h"
#include "creategroupdialog.h"
#include <QDateTime>
#include <unistd.h>
#include "recvfiledialog.h"
#include "filesender.h"
#include "filepkg.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //initialize
    ui->setupUi(this);
    setWindowTitle("Go Chat");
    ui->fileBtn->setIcon(QIcon(":/img/file.png"));
    ui->sendBtn->setIcon(QIcon(":/img/send.png"));
    ui->chatwidget->setStyleSheet("border-radius:15px");

    //udp connect
    udpSocket = new QUdpSocket();
    this->ipStr="127.0.0.1";
    this->port=9999;
    this->recvPort=9996;

    udpSocket->bind(QHostAddress::AnyIPv4,recvPort);
    QObject::connect(udpSocket,SIGNAL(readyRead()),this,SLOT(recvData()));

    //TODO

    //a new friend or group
//    UserInfo *fri=new UserInfo(1,"zzz","hhh");
//    GroupInfo *gro=new GroupInfo(2,"ccc","lll");

    headbtn = new QToolButton();

    ui->chatwidget->hide();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_fileBtn_clicked()
{
    QString dirName = QFileDialog::getOpenFileName();


        UserInfo user;
        QByteArray tmp="";
        FileMeta File(user,dirName);
        qDebug()<<"name     :"<<File.getName();
        msg file_signal("1002",File.toQByteArray());
        //send
        udpSocket->writeDatagram(file_signal.toQByteArray(), QHostAddress(sendipStr.toStdString().c_str()),recvPort);
        //TODO
        FileMeta File_rec;
            msg file_get;
            file_get=file_get.fromQByteArray(file_signal.toQByteArray());
            File_rec=File_rec.fromQByteArray(file_get.content);
            QString Filename=File_rec.getName();
            qDebug()<<"filename::"<<Filename;
        qDebug()<<"dirname:"<<dirName;
        usleep(1000);

    filesender m_filesender;
    m_filesender.comeOnfile(dirName);
    //TODO
    qDebug()<<"dirname:"<<dirName;
}


void MainWindow::on_addfriendBtn_clicked()
{
    AddfriendDialog afd;
    afd.exec();

}


void MainWindow::on_addgroupBtn_clicked()
{
    AddgroupDialog agd;
    agd.exec();
}


void MainWindow::on_sendBtn_clicked()
{
    QString txt = ui->textEdit->text();

        //chat msg
        ChatMessage cmsg(Myid,sendid,txt);
        msg send_cmsg1("1000",cmsg.toQByteArray());
        msg send_cmsg2("1001",cmsg.toQByteArray());
        //send
        if(sendipStr=="0"){
            udpSocket->writeDatagram(send_cmsg2.toQByteArray(), QHostAddress(QHostAddress::Broadcast),recvPort);
            QDateTime dateTime= QDateTime::currentDateTime(); // get time
            QString time = dateTime .toString("yyyy-MM-dd hh:mm:ss"); // normalization

            ui->textBrowser->insertPlainText(time+" Me\n"+txt+"\n");
        }

        else{
            udpSocket->writeDatagram(send_cmsg1.toQByteArray(), QHostAddress(sendipStr.toStdString().c_str()),recvPort);
            QDateTime dateTime= QDateTime::currentDateTime(); // get time
            QString time = dateTime .toString("yyyy-MM-dd hh:mm:ss"); // normalization

            ui->textBrowser->setAlignment(Qt::AlignRight);//set right
            ui->textBrowser->insertPlainText(time+" Me\n"+txt+"\n");

        }
        //TODO
        qDebug()<<"send textEdit:"<<txt;
        ui->textEdit->clear();
        ui->textEdit->setFocus();
}
void MainWindow::updatefrilist(QList<UserInfo> flist){
    if(flist.size()!=0){
        listbtn = new QToolButton();
        listbtn->setText(flist[flist.size()-1].getName());
        listbtn->setIcon(QIcon(":/img/snoopy7.png"));
        listbtn->setIconSize(QSize(40,40));
        listbtn->setAutoRaise(true);
        listbtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        listbtn->setFixedSize(190,70);
        ui->friendsLayout->addWidget(listbtn);
        friendlist.push_back(listbtn);
    }

    headbtn->setStyleSheet("color:white");
    if (flist.size()!=0){
        connect(friendlist[flist.size()-1],&QToolButton::clicked,[=](){
            headbtn->setText(flist[flist.size()-1].getName());
            headbtn->setIcon(QIcon(":/img/snoopy7.png"));
            headbtn->setIconSize(QSize(100,100));
            headbtn->setAutoRaise(true);
            headbtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            headbtn->setFixedSize(150,30);
            ui->textBrowser->clear();
            ui->headLayout->addWidget(headbtn);
            ui->chatwidget->show();
            //send id
            QByteArray id_tmp;
            quint32 id=flist[flist.size()-1].getID();
            id_tmp.setNum(id);
            msg msg_tosend("0003",id_tmp);
            //send
            udpSocket->writeDatagram(msg_tosend.toQByteArray(), QHostAddress(ipStr.toStdString().c_str()),port);
            //set sendid
            sendid=id;
            qDebug()<<"send 0003 success "<<sendid;
        });
    }
}

void MainWindow::updategrolist(QList<GroupInfo> glist){
    if(glist.size()!=0){
        listbtn = new QToolButton();
        listbtn->setText(glist[glist.size()-1].getName());
        listbtn->setIcon(QIcon(":/img/snoopy11.jpeg"));
        listbtn->setIconSize(QSize(40,40));
        listbtn->setAutoRaise(true);
        listbtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        listbtn->setFixedSize(190,70);
        ui->groupsLayout->addWidget(listbtn);
        grouplist.push_back(listbtn);
    }
   headbtn->setStyleSheet("color:white");
    if(grouplist.size()!=0){
        connect(grouplist[grouplist.size()-1],&QToolButton::clicked,[=](){
            headbtn->setText(glist[grouplist.size()-1].getName());
            headbtn->setIcon(QIcon(":/img/snoopy11.jpeg"));
            headbtn->setAutoRaise(true);
            headbtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            headbtn->setFixedSize(150,30);
            ui->textBrowser->clear();
            ui->headLayout->addWidget(headbtn);
            ui->chatwidget->show();
            //send id
            QByteArray id_tmp;
            quint32 id=glist[grouplist.size()-1].getID();
            id_tmp.setNum(id);
            msg msg_tosend("0007",id_tmp);
            //send
            udpSocket->writeDatagram(msg_tosend.toQByteArray(), QHostAddress(ipStr.toStdString().c_str()),port);
            //set sendgid
            sendgid=id;
            //这个是群聊时候发言人的id
            sendipStr="0";
        });
    }
}

void MainWindow::updatelist(QList<UserInfo> flist,QList<GroupInfo> glist)
{
    /*
        if(flist.size()!=0){
        listbtn = new QToolButton();
        listbtn->setText(flist[flist.size()-1].getName());
        listbtn->setIcon(QIcon(":/img/snoopy7.png"));
        listbtn->setIconSize(QSize(40,40));
        listbtn->setAutoRaise(true);
        listbtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        listbtn->setFixedSize(190,70);
        ui->friendsLayout->addWidget(listbtn);
        friendlist.push_back(listbtn);
    }
    if(glist.size()!=0){
        listbtn = new QToolButton();
        listbtn->setText(glist[glist.size()-1].getName());
        listbtn->setIcon(QIcon(":/img/snoopy11.jpeg"));
        listbtn->setIconSize(QSize(40,40));
        listbtn->setAutoRaise(true);
        listbtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        listbtn->setFixedSize(190,70);
        ui->groupsLayout->addWidget(listbtn);
        grouplist.push_back(listbtn);
    }

    headbtn->setStyleSheet("color:white");
    if (flist.size()!=0){
        connect(friendlist[flist.size()-1],&QToolButton::clicked,[=](){
            headbtn->setText(flist[flist.size()-1].getName());
            headbtn->setIcon(QIcon(":/img/snoopy7.png"));
            headbtn->setIconSize(QSize(100,100));
            headbtn->setAutoRaise(true);
            headbtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            headbtn->setFixedSize(150,30);
            ui->textBrowser->clear();
            ui->headLayout->addWidget(headbtn);
            ui->chatwidget->show();
            //send id
            QByteArray id_tmp;
            quint32 id=flist[flist.size()-1].getID();
            id_tmp.setNum(id);
            msg msg_tosend("0003",id_tmp);
            //send
            udpSocket->writeDatagram(msg_tosend.toQByteArray(), QHostAddress(ipStr.toStdString().c_str()),port);
            //set sendid
            sendid=id;
            qDebug()<<"send 0003 success "<<sendid;
        });

    }

    if(grouplist.size()!=0){
        connect(grouplist[grouplist.size()-1],&QToolButton::clicked,[=](){
            headbtn->setText(glist[grouplist.size()-1].getName());
            headbtn->setIcon(QIcon(":/img/snoopy11.jpeg"));
            headbtn->setAutoRaise(true);
            headbtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            headbtn->setFixedSize(150,30);
            ui->textBrowser->clear();
            ui->headLayout->addWidget(headbtn);
            ui->chatwidget->show();
            //send id
            QByteArray id_tmp;
            quint32 id=glist[grouplist.size()-1].getID();
            id_tmp.setNum(id);
            msg msg_tosend("0007",id_tmp);
            //send
            udpSocket->writeDatagram(msg_tosend.toQByteArray(), QHostAddress(ipStr.toStdString().c_str()),port);
            //set sendgid
            sendgid=id;
            //这个是群聊时候发言人的id
            sendipStr="0";
        });
    }
    */
}

void MainWindow::recvData(){
    quint32 if_receive=1;
    while(udpSocket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());

        msg msg_get;
        msg_get=msg_get.fromQByteArray(datagram);

        if(msg_get.flag=="1000"){
            ChatMessage get_cmsg;
            get_cmsg=get_cmsg.fromQByteArray(msg_get.content);
            get_cmsg.getSender();//该用户给我发来了信息
            for (int i=0;i<flist.size() ;i++ ) {
                if(get_cmsg.getSender()==flist[i].getID()){
                    QDateTime dateTime= QDateTime::currentDateTime(); // get time
                      QString time = dateTime .toString("yyyy-MM-dd hh:mm:ss"); // normalization
                     //time给了
                    ui->textBrowser->setAlignment(Qt::AlignLeft);//set right
                    ui->textBrowser->insertPlainText(time+" "+flist[i].getName()+"\n"+get_cmsg.getContent()+"\n");
                    break;
                }
            }
//            ui->textBrowser->setAlignment(Qt::AlignLeft);
//            ui->textBrowser->insertPlainText(get_cmsg.getContent()+"\n");

        }
        else if(msg_get.flag=="1001"){
            ChatMessage get_cmsg;
            get_cmsg=get_cmsg.fromQByteArray(msg_get.content);
            //get_cmsg.getSender();//该group给我发来了信息

            if(if_receive){
                for (int i=0;i<flist.size() ;i++ ) {
                    if(get_cmsg.getSender()==flist[i].getID()){
                        QDateTime dateTime= QDateTime::currentDateTime(); // get time
                        QString time = dateTime .toString("yyyy-MM-dd hh:mm:ss "); // normalization
                         //time给了
                        ui->textBrowser->setAlignment(Qt::AlignLeft);
                        ui->textBrowser->insertPlainText(time+" "+flist[i].getName()+"\n"+get_cmsg.getContent()+"\n");
                        break;
                    }
                }
            }
        }
        else if(msg_get.flag=="S0003"){
            UserInfo Afriend;
            Afriend=Afriend.fromQByteArray(msg_get.content);
            flist.append(Afriend);
            qDebug()<<"S0003_information:"<<Afriend.getID();
            updatefrilist(flist);
        }
        else if(msg_get.flag=="S0004"){
            GroupInfo AGroup;
            AGroup=AGroup.fromQByteArray(msg_get.content);
            glist.append(AGroup);
            updategrolist(glist);
        }
        else if(msg_get.flag=="S0005"){
            //此处应该给的是个全局变量，表示我当前要发送信息对象的IP
            sendipStr=msg_get.IP;
            qDebug()<<"S0005_informaton"<<sendipStr;
        }
        else if(msg_get.flag=="S0006"){
            //if_receive=msg_get.content.toUInt();
        }
        else if(msg_get.flag=="S0007"){
            UserInfo new_friend;

            new_friend=new_friend.fromQByteArray(msg_get.content);
            flist.append(new_friend);
            qDebug()<<"S0007_information"<<new_friend.getID();
            updatefrilist(flist);
        }
        else if(msg_get.flag=="S0008"){
            UserInfo new_group;
            new_group=new_group.fromQByteArray(msg_get.content);
            glist.append(new_group);
            updategrolist(glist);
        }
        else if(msg_get.flag=="S0009"){

            msg_get=msg_get.fromQByteArray(datagram);
            GroupInfo AGroup;
            AGroup=AGroup.fromQByteArray(msg_get.content);
            quint32 id=AGroup.getID();
            QMessageBox::information(this,tr("success!"),
                                 tr("your group id is %1").arg(id),
                                 QMessageBox::Yes);
            glist.append(AGroup);
            updategrolist(glist);
        }
        else if(msg_get.flag=="1002"){
            FileMeta File_rec;
            File_rec=File_rec.fromQByteArray(msg_get.content);
            QString Filename=File_rec.getName();


            RecvfileDialog rvfd(Filename);
            rvfd.exec();
        }
        else{
            qDebug()<<"flag error!!";
        }
        //TODO
    }
}
void MainWindow::sendData(QString sendStr)
{

   udpSocket->writeDatagram(
                sendStr.toStdString().c_str(),
                QHostAddress(ipStr.toStdString().c_str()),
                port);
}

void MainWindow::on_creategroupBtn_clicked()
{
    CreategroupDialog cgdl;
    cgdl.exec();
}
