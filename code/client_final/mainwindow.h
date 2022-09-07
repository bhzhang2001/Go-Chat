#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QUdpSocket>
#include "userinfo.h"
#include "groupinfo.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    int port;
    int recvPort;
    QString ipStr;
    QString sendipStr;
    quint32 sendid;
    quint32 sendgid;
    quint32 Myid;
    QUdpSocket *udpSocket;
    QList<UserInfo> flist;
    QList<GroupInfo> glist;
    //QString msg;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_fileBtn_clicked();

    void on_addfriendBtn_clicked();

    void on_addgroupBtn_clicked();

    void on_sendBtn_clicked();

    void updatelist(QList<UserInfo> flist,QList<GroupInfo> glist);
    void updatefrilist(QList<UserInfo> flist);
    void updategrolist(QList<GroupInfo> glist);
    void recvData();
    void sendData(QString sendStr);

    void on_creategroupBtn_clicked();

private:
    Ui::MainWindow *ui;
    QToolButton * listbtn;
    QToolButton * headbtn;
    QVector <QToolButton *> friendlist;
    QVector <QToolButton *> grouplist;
};
#endif // MAINWINDOW_H
