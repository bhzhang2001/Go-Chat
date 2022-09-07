#ifndef SUBMITFRIENDDIALOG_H
#define SUBMITFRIENDDIALOG_H

#include <QDialog>
#include <QUdpSocket>
namespace Ui {
class SubmitfriendDialog;
}

class SubmitfriendDialog : public QDialog
{
    Q_OBJECT

public:
    int port;
    QString ipStr;
    QUdpSocket *udpSocket;
    explicit SubmitfriendDialog(QWidget *parent = nullptr,QString msg="");
    ~SubmitfriendDialog();

private slots:
    void on_acceptBtn_clicked();

    void on_rejectBtn_clicked();

    void sendData(QString sendStr);

private:
    Ui::SubmitfriendDialog *ui;
};

#endif // SUBMITFRIENDDIALOG_H
