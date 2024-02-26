#include "mainwindow.h"
#include <QApplication>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
//#include <string.h>
#include <arpa/inet.h>
#include <future>
#include <thread>

#define PORT 2024
#define ADRR "127.0.0.1"

extern int errno;
int sd;
char MessageFromServer[1000];
char MessageForVerify [1000];
char MessageToServer  [1000];
char ThisUser          [20];
char ToUser            [20];
char AncestorMessage   [50];
char DateLastMessage   [30];
bool ReplyStatus;

void Client_Execution ()
{
    struct sockaddr_in server;
    int Read_Code;

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("[CLIENT] Error socket\n");
        return;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ADRR);
    server.sin_port = htons(PORT);

    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1){
        perror("[CLIENT] Error connect\n");
        return;
    }

    while(1){
        if(strcmp(MessageToServer, MessageForVerify) != 0){
            printf("[CLIENT]MESSAGE TO SERVER --------------\n%s\n--------------\n", MessageToServer); fflush(stdout);
            if (send(sd, MessageToServer, strlen(MessageToServer), 0) <= 0)
            {
                perror("[CLIENT]Eroare la send() spre server.\n");
                return;
            }
            bzero(MessageToServer, sizeof(MessageToServer));

            if (-1 == (Read_Code = recv(sd, MessageFromServer, sizeof(MessageFromServer), 0)))
                perror("[CLIENT] Error reading from socket\n");
            MessageFromServer[Read_Code] = '\0';

            printf("[CLIENT]MESSAGE FROM SERVER --------------\n%s\n--------------\n", MessageFromServer); fflush(stdout);
        }
    }
    close(sd);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto thread = std::async(Client_Execution);
    MainWindow w;
    w.show();
    return a.exec();
}
