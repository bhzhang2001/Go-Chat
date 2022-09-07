#ifndef FILESENDER_H
#define FILESENDER_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QFile>
#include "imageframehead.h"
#define TRAN_SIZE 1024;


QT_BEGIN_NAMESPACE
namespace Ui { class server; }
QT_END_NAMESPACE


class filesender : public QMainWindow
{
    Q_OBJECT

public:
    explicit filesender(QWidget *parent = nullptr);
    ~filesender();
    void comeOnfile(QString path);

private:
    Ui::server *ui;
    QUdpSocket *m_udpSocket;



    qint64 sendsize;//已发送文件大小
    QString fileName;//文件名字
    qint64 fileSize;//文件大小
    QFile file;//file是要接收并存储的文件

};

#endif // FILESENDER_H
