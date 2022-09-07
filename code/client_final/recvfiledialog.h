#ifndef RECVFILEDIALOG_H
#define RECVFILEDIALOG_H

#include <QDialog>
#include "u.h"
#include<QFile>
namespace Ui {
class RecvfileDialog;
}

class RecvfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecvfileDialog(QString,QWidget *parent = nullptr);
    ~RecvfileDialog();
private slots:
    void slotRecv_client(char *buf, int len);
private:
    Ui::RecvfileDialog *ui;
    UdpThread *m_udpThread;

    QFile file;
    qint64 filesize;
    QString filename;
};

#endif // RECVFILEDIALOG_H
