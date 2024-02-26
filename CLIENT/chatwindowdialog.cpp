#include "chatwindowdialog.h"
#include "ui_chatwindowdialog.h"
#include "userslistdialog.h"
#include "toolbox.h"
#include <QMessageBox>
#include <QDebug>
#include <QTreeWidgetItem>
#include "qlabel.h"

ChatWindowDialog::ChatWindowDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatWindowDialog)
{
    ui->setupUi(this);
    QLabel *Username = new QLabel; QFont settext;
    settext.setBold(true);
    Username->setFont(settext);
    Username->setText(QString::fromStdString(ToUser));
    ui->verticalLayout->addWidget(Username);
    bzero(MessageFromServer, sizeof(MessageFromServer));
    bzero(MessageToServer, sizeof(MessageToServer));
    char request[50];
    sprintf(request, "HISTORY:%s:%s", ThisUser, ToUser);
    strcpy(MessageToServer, request);
    ui->MessageList->setColumnCount(1); //setam dimensiunea spatiului folosit pentru mesajele noi primite
    sleep(1);
    if (strcmp(MessageFromServer, "No messages in the conversation") != 0) {


    char *line = strtok(MessageFromServer, "\n");
    while (line != NULL) {
        char message[100];
        char ancestor_message[100];

        if (sscanf(line, "/%99[^/]/%99[^\n]", ancestor_message, message) == 2) {
            QTreeWidgetItem* mess = new QTreeWidgetItem(ui->MessageList);
            char Message[100];

            if (strcmp(ancestor_message, "-") == 0) {
                sprintf(Message, "%s", message);
            } else {
                sprintf(Message, "<%s> %s", ancestor_message, message);
            }
            mess->setText(0,Message);
            ui->MessageList->addTopLevelItem(mess);
        } else {
            strcpy(DateLastMessage, line);
        }
        line = strtok(NULL, "\n");
    }
    } else {
        strcpy(DateLastMessage, "2000-10-12 00:00:00");
    }
    chatManager = new ChatManager(this);
    connect(chatManager, SIGNAL(aNewMessage(QString)), this, SLOT(HandleMessageList(QString)));
    chatManager->start();
}


void ChatWindowDialog::on_SendButton_clicked()
{
    char new_message[100], request[300];
    strcpy(new_message, ui->MessageBox->text().toStdString().c_str());
    if (strcmp(AncestorMessage, "") == 0) {
        sprintf(request, "SEND/%s/%s/%s/%d/-", new_message, ThisUser, ToUser, ReplyStatus);
    } else {
        sprintf(request, "SEND/%s/%s/%s/%d/%s", new_message, ThisUser, ToUser, ReplyStatus, AncestorMessage);
        strcpy(AncestorMessage, "");
        ReplyStatus = 0;
    }
    bzero(MessageToServer, sizeof(MessageToServer));
    bzero(MessageToServer, sizeof(MessageToServer));
    strcpy(MessageToServer, request); //ar trebuie sa adaug si chestia ai cu reply, gen ancestor_message sau ceva
    sleep(1);
    ui->MessageBox->clear();
}


void ChatWindowDialog::on_MessageList_itemClicked(QTreeWidgetItem *item, int column)
{
    char mess[100];
    strcpy(mess,item->text(0).toStdString().c_str());
   // strcpy(mess, "<sabrina: ce faci> Me: nimic, stau, tu?");

    char *start = strstr(mess, ">");
    if (start != NULL) {
        char extractedMessage[100];
        size_t leadingSpaces = strspn(start + 1, " \t\n\r");
        start += leadingSpaces;
        strcpy(extractedMessage, start + 1);
        strcpy(AncestorMessage, extractedMessage);
        ReplyStatus = 1;
    } else {
        strcpy(AncestorMessage, item->text(0).toStdString().c_str());
        ReplyStatus = 1;
    }
}

void ChatWindowDialog::HandleMessageList(QString message)
{
    // Implement the behavior when a new message is received
    ui->MessageList->setColumnCount(1);
    QTreeWidgetItem* NewMessage = new QTreeWidgetItem(ui->MessageList);
    char mess[100];
    //strcpy(mess, message.toStdString().c_str());

    NewMessage->setText(0, message.toStdString().c_str());
    ui->MessageList->addTopLevelItem(NewMessage);
    ReplyStatus = 0;
    ui->MessageList->selectionModel()->clearSelection();
}

void ChatWindowDialog::on_BackButton_clicked()
{
    //optim threadul care se ocupa de mesaje primite in timp real
    chatManager->Stop = true;
    chatManager->terminate();

    //ne intoarcem la lista userilor
    this->close();
    bzero(MessageToServer, sizeof(MessageToServer));
    bzero(MessageFromServer, sizeof(MessageFromServer));
    strcpy(MessageToServer, "LIST_ALL_USERS");
    sleep(1);
    UsersListDialog UsersListDialog;
    UsersListDialog.setModal(true);
    UsersListDialog.exec();
}

