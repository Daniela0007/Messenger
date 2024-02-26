QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chatmanager.cpp \
    chatwindowdialog.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    registerdialog.cpp \
    userslistdialog.cpp

HEADERS += \
    chatmanager.h \
    chatwindowdialog.h \
    logindialog.h \
    mainwindow.h \
    registerdialog.h \
    toolbox.h \
    userslistdialog.h

FORMS += \
    chatwindowdialog.ui \
    logindialog.ui \
    mainwindow.ui \
    registerdialog.ui \
    userslistdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
