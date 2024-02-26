#include "userslistdialog.h"
#include "ui_userslistdialog.h"
#include "toolbox.h"
#include "qlabel.h"
#include "chatwindowdialog.h"
#include <QMessageBox>

UsersListDialog::UsersListDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UsersListDialog)
{
    ui->setupUi(this);
    char Users[20][20];
    char *token = strtok(MessageFromServer, "\n");
    int count_users = 0;
    while (token != NULL) {
        strcpy(Users[count_users], token);
        token = strtok(NULL, "\n");
        count_users++;
    }
    int i = 0;
    while (i<count_users) {
        if (strcmp(Users[i], ThisUser) != 0) {
            QPushButton *USERS = new QPushButton(QString::fromStdString(Users[i]), this);
            ui->verticalLayout_2->addWidget(USERS);
            connect(USERS, &QPushButton::clicked, [this,USERS]() {
                GO_TO_CONVERSATION(USERS->text());
            });
        }
        i++;
    }
}

void UsersListDialog::GO_TO_CONVERSATION(const QString &textButton) {
    strcpy(ToUser, textButton.toStdString().c_str());
    this->close();
    ChatWindowDialog ChatWindowDialog;
    ChatWindowDialog.setModal(true);
    ChatWindowDialog.exec();
}

UsersListDialog::~UsersListDialog()
{
    delete ui;
}

void UsersListDialog::on_EXIT_BUTTON_clicked()
{
    QMessageBox::information(this, "LOGOUT", "You are now logged off.");
    this->close();
}

