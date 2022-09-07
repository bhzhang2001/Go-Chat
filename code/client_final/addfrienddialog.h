#ifndef ADDFRIENDDIALOG_H
#define ADDFRIENDDIALOG_H

#include <QDialog>
#include <QUdpSocket>
namespace Ui {
class AddfriendDialog;
}

class AddfriendDialog : public QDialog
{
    Q_OBJECT

public:
    int port;
    QString ipStr;
    QUdpSocket *udpSocket;
    explicit AddfriendDialog(QWidget *parent = nullptr);
    ~AddfriendDialog();

private slots:
    void sendData(QString sendStr);
    void on_pushButton_clicked();

private:
    Ui::AddfriendDialog *ui;
};

#endif // ADDFRIENDDIALOG_H
