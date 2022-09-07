#ifndef CREATEGROUPDIALOG_H
#define CREATEGROUPDIALOG_H

#include <QDialog>
#include <QUdpSocket>
namespace Ui {
class CreategroupDialog;
}

class CreategroupDialog : public QDialog
{
    Q_OBJECT

public:
    int port;
    QString ipStr;
    QUdpSocket *udpSocket;
    explicit CreategroupDialog(QWidget *parent = nullptr);
    ~CreategroupDialog();

private slots:
    void on_createBtn_clicked();

private:
    Ui::CreategroupDialog *ui;
};

#endif // CREATEGROUPDIALOG_H
