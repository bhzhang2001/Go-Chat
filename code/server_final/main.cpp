#include "mainwindow.h"
#include "server.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    server ser;
    ser.exec();
    MainWindow w;
    w.show();
    return a.exec();
}
