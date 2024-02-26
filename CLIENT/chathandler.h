#ifndef CHATHANDLER_H
#define CHATHANDLER_H
#include <QThread>

class ChatHandler : public QThread
{
    Q_OBJECT
public:
     ChatHandler(QObject *parent = 0, bool b = false);
    void run();

    bool STOP;

signals:
    void NewMessageReceived(QString);
public slots:

};

#endif // CHATHANDLER_H


/*#ifndef CHATTHREAD_H
#define CHATTHREAD_H

#include <QThread>

class ChatThread : public QThread
{
    Q_OBJECT
public:
    explicit ChatThread(QObject *parent = 0, bool b = false);
    void run();

    // if Stop = true, the thread will break
    // out of the loop, and will be disposed
    bool Stop;

signals:
    // To communicate with Gui Thread
    // we need to emit a signal
    void aNewMessage(QString);

public slots:

};

#endif // MYTHREAD_H
*/
