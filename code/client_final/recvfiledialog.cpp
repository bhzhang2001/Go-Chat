#include "recvfiledialog.h"
#include "ui_recvfiledialog.h"

RecvfileDialog::RecvfileDialog(QString f_name,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecvfileDialog)
{
    ui->setupUi(this);
    m_udpThread = new UdpThread(this);
    //sigRecvok:线程完成——读取结束的信号
    //信号传来了数据char
    this->filename=f_name;
    ui->textBrowser->insertPlainText("receiving file:"+f_name+"\n");
    ui->textBrowser->insertPlainText("waiting...\n");
    connect(m_udpThread, SIGNAL(sigRecvOk(char*,int)), this, SLOT(slotRecv_client(char*,int)));
    m_udpThread->start();
    qDebug("OK");
}

RecvfileDialog::~RecvfileDialog()
{
    delete ui;
}


void RecvfileDialog::slotRecv_client(char * buf, int len)
{
    file.setFileName(filename);
    filesize = len;
    file.resize(filesize);

    bool isOK = file.open(QIODevice::WriteOnly);//isOK:文件成功打开标志

    qDebug()<<isOK;
    qint64 write_len=file.write(buf,len);

    qDebug()<<"client：写入文件的大小"<<write_len;
    qDebug()<<"filesize="<<len;

//    QPixmap pixmap;
//    pixmap.loadFromData((uchar*)buf, len, "ts");
//    ui->label->setPixmap(pixmap);
    file.close();
    ui->textBrowser->insertPlainText("end");
}
