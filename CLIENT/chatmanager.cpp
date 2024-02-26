#include "chatmanager.h"
#include "qmutex.h"
#include "toolbox.h"

ChatManager::ChatManager(QObject *parent, bool b) :
    QThread(parent), Stop(b)
{
}

void ChatManager::run()
{
    sleep(2);
    do {
        QMutex mutex;
        mutex.lock();
        if(this->Stop) break;
        mutex.unlock();
        //get all the new messages in real time
        char request[100];
        printf("ULTIMA DATA A MESAJULUI ESTE %s\n", DateLastMessage);
        sprintf(request, "GET/%s/%s/%s", DateLastMessage, ThisUser, ToUser);
        bzero(MessageFromServer, sizeof(MessageFromServer));
        bzero(MessageToServer, sizeof(MessageToServer));
        strcpy(MessageToServer, request);
        sleep(1);


        char message[100];
        char ancestor_message[100];
        char Message[100];
        if (strstr(MessageFromServer, "Inbox currently empty")) {
            printf("Am gasit ca inboxul este empty\n"); fflush(stdout);
            this->sleep(2);
            continue;
        }
        char *line = strtok(MessageFromServer, "\n");
        if (sscanf(line, "/%99[^/]/%99[^\n]", ancestor_message, message) == 2) {
            if (strcmp(ancestor_message, "-") == 0) {
                sprintf(Message, "%s", message);
            } else {
                sprintf(Message, "<%s> %s", ancestor_message, message);
            }
            //printf("Ancestor_Message: %s\n", ancestor_message); fflush(stdout);
            //printf("Message: %s\n", message); fflush(stdout);
            line = strtok(NULL, "\n");
            strcpy(DateLastMessage, line);
            printf("DATELASTIME CHANGED TO %s", DateLastMessage); fflush(stdout);
            ReplyStatus = 0;
            emit aNewMessage(Message);
        }

        this->sleep(2);
    }while(!Stop);
   // printf("S-a oprit threadul, dont know why\n"); fflush(stdout);
}
