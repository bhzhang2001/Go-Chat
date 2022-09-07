QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addfrienddialog.cpp \
    addgroupdialog.cpp \
    creategroupdialog.cpp \
    filepkg.cpp \
    filesender.cpp \
    groupinfo.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    msg.cpp \
    recvfiledialog.cpp \
    registerdialog.cpp \
    submitfrienddialog.cpp \
    u.cpp \
    udpconnect.cpp \
    userinfo.cpp \
    chatmessage.cpp

HEADERS += \
    addfrienddialog.h \
    addgroupdialog.h \
    creategroupdialog.h \
    filepkg.h \
    filesender.h \
    groupinfo.h \
    imageframehead.h \
    logindialog.h \
    mainwindow.h \
    msg.h \
    recvfiledialog.h \
    registerdialog.h \
    submitfrienddialog.h \
    u.h \
    udpconnect.h \
    userinfo.h \
    chatmessage.h

FORMS += \
    addfrienddialog.ui \
    addgroupdialog.ui \
    creategroupdialog.ui \
    logindialog.ui \
    mainwindow.ui \
    recvfiledialog.ui \
    registerdialog.ui \
    submitfrienddialog.ui

CONFIG += c++11
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    UI.qrc
