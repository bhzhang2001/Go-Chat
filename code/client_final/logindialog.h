#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QUdpSocket>
#include "userinfo.h"
#include "groupinfo.h"
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    QString l;
    int port;
    int recvPort;
    QString ipStr;
    QUdpSocket *udpSocket;
    QString message;

    QList<UserInfo> flist;
    QList<GroupInfo> glist;
    quint32 Myid;

    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_loginBtn_clicked();

    void on_registerBtn_clicked();

    void recvData();

    void sendData(QString sendStr);

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
