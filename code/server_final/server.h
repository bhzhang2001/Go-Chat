#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QString>
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
#include <QToolButton>
#include <QUdpSocket>

namespace Ui {
class server;
}

class server : public QDialog
{
    Q_OBJECT

public:
    explicit server(QWidget *parent = nullptr);
    ~server();

private slots:
    void on_send_message_Btn_clicked();
    void recvData();  

private:
    Ui::server *ui;
    QString server_ip;
    QString server_port;
    quint32 MaxID = 10001;

    struct hb_msg{
        quint32 ID;
        QString IP;
    };
    QList<struct hb_msg> ID_IP_list;

    //QTcpServer *tcpSocket_s;
    //QTcpSocket *tcpSocket;

    QList<QUdpSocket*> socket_list;  // socket list
    QVector <QToolButton *> friendlist;

    QUdpSocket *udpSocket;


};

#endif // SERVER_H
