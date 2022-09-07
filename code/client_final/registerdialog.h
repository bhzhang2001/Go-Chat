#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QUdpSocket>
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    int port;
    int recvPort;
    QString ipStr;
    QUdpSocket *udpSocket;
    QString message;
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_registerBtn_clicked();

    void recvData();

private:
    Ui::RegisterDialog *ui;
    bool check_usrname(QString usr);
    bool check_pwd(QString pwd);
};

#endif // REGISTERDIALOG_H
