#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    LoginDialog dlg;
    if(dlg.exec()==QDialog::Accepted){
        //w.l = dlg.l;
        w.Myid=dlg.Myid;
        w.show();
        return a.exec();
    }
}
