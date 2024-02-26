#include "logindialog.h"
#include "ui_logindialog.h"
#include "userslistdialog.h"
#include "toolbox.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_LoginButton_clicked()
{
    if (ui->PasswordLine->text().isEmpty() || ui->PasswordLine->text().isEmpty()) {
        QMessageBox::warning(this, "WindowsError", "All fields need to be completed!");
    } else {
        char username[20], password[20];
        strncpy(username, ui->UsernameLine->text().toStdString().c_str(), sizeof(username));
        strncpy(password, ui->PasswordLine->text().toStdString().c_str(), sizeof(password));

        // printf("the username is %s \n", username); fflush(stdout);
        // printf("the password is %s \n", password); fflush(stdout);
        char DataSend[48];
        sprintf(DataSend, "LOGIN:%s:%s", username, password);
        strcpy(MessageToServer, DataSend);
       // printf("the password is %s \n", password); fflush(stdout);
        sleep(1); //wait for the server
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle("LOGIN");
        msgBox.setText(MessageFromServer);
        msgBox.setStyleSheet("QMessageBox{ background-color: #FAD02E; }"
                             "QMessageBox QLabel{ color: #333; }");
        msgBox.exec();
        if(strstr(MessageFromServer, "Successfully logged in\n"))
        {
            this->close();
            strcpy(ThisUser, username);
            bzero(MessageFromServer, sizeof(MessageFromServer));
            bzero(MessageToServer, sizeof(MessageToServer));
            strcpy(MessageToServer, "LIST_ALL_USERS");
            sleep(1);
           // this->close();
            UsersListDialog UsersListDialog;
            UsersListDialog.setModal(true);
            UsersListDialog.exec();
        }
    }

}

