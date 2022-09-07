#ifndef FILEPKG_H
#define FILEPKG_H
#include <QObject>
#include <userinfo.h>


//整理文件名和文件内容
class FileMeta
{
public:
    //在生成文件时指定user
    FileMeta(){
        unique_name="";original_name="";
    }
    FileMeta(UserInfo user, QString original_name = "");

    QString toQString();
    static FileMeta fromQByteArray(QByteArray msg_content);
    QByteArray toQByteArray();

    QString getName(){ return unique_name;};
    QString getBaseName(){return original_name;};

private:

    QString unique_name;
    QString original_name;

};



#endif // FILEPKG_H
