#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "qthread.h"
class ChatManager : public QThread
{
    Q_OBJECT
public:
    explicit ChatManager(QObject *parent = 0, bool b = false);
    void run();
    bool Stop;
signals:
    void aNewMessage(QString);
public slots:

};

#endif // CHATMANAGER_H
