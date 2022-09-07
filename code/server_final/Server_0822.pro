QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chatmessage.cpp \
    datadb.cpp \
    group_chat_message.cpp \
    groupinfo.cpp \
    main.cpp \
    mainwindow.cpp \
    msg.cpp \
    server.cpp \
    user.cpp \
    userinfo.cpp

HEADERS += \
    chatmessage.h \
    database_all.h \
    datadb.h \
    group_chat_message.h \
    groupinfo.h \
    mainwindow.h \
    msg.h \
    server.h \
    user.h \
    userinfo.h

FORMS += \
    mainwindow.ui \
    server.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    ../Server_hhh_jjj/Server_0822.pro

DISTFILES +=
