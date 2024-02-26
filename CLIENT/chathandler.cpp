#include "chathandler.h"
#include <toolbox.h>
#include <QThread>
#include <QDebug>
#include <QtCore>
#include <QMessageBox>
#include <QTreeWidgetItem>

ChatHandler::ChatHandler(QObject *parent, bool b) : QThread(parent), STOP(b)
{
}

void ChatHandler::run() {
    sleep(2);
    //QMutex mutex am putea sa il punem aici???
    do {
        QMutex mutex;
        mutex.lock();
        if (this->STOP) break;
        mutex.unlock();
        char request[100];
        sprintf(request, "GET:%s:%s:%s", DateLastMessage, ThisUser, ToUser);
        printf("%s---\n" , request);
        bzero(MessageToServer, sizeof(MessageToServer));
        bzero(MessageFromServer, sizeof(MessageFromServer));
       // strcpy(MessageToServer, request);
        char response[100];
        strcpy(response, "aici e mesajul inserat on purpose");

        QString the_signal = QString(response);
        ReplyStatus = 0;
        emit NewMessageReceived(response);
    }while (!STOP);
}

