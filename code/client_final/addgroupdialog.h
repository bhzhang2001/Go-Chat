#ifndef ADDGROUPDIALOG_H
#define ADDGROUPDIALOG_H

#include <QDialog>
#include <QUdpSocket>
namespace Ui {
class AddgroupDialog;
}

class AddgroupDialog : public QDialog
{
    Q_OBJECT

public:
    int port;
    QString ipStr;
    QUdpSocket *udpSocket;
    explicit AddgroupDialog(QWidget *parent = nullptr);
    ~AddgroupDialog();

private slots:
    void sendData(QString sendStr);
    void on_addBtn_clicked();

private:
    Ui::AddgroupDialog *ui;
};

#endif // ADDGROUPDIALOG_H
