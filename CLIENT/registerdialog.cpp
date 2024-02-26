#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "toolbox.h"
#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_RegisterButton_clicked()
{
    if (ui->UsernameLine->text().isEmpty() || ui->PasswordLine->text().isEmpty() || ui->PasswordVerifyLine->text().isEmpty()){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("ERROR");
        msgBox.setText("All fields are mandatory and must be filled out.");
        msgBox.setStyleSheet("QMessageBox{ background-color: #FAD02E; }"
                             "QMessageBox QLabel{ color: #333; }");
        msgBox.exec();

    } else {
        char username[20], password[20], password_verify[20];
        strncpy(username, ui->UsernameLine->text().toStdString().c_str(), sizeof(username));
        strncpy(password, ui->PasswordLine->text().toStdString().c_str(), sizeof(password));
        strncpy(password_verify, ui->PasswordVerifyLine->text().toStdString().c_str(), sizeof(password_verify));

        char DataSend[60];
        sprintf(DataSend, "REGISTER:%s:%s:%s", username, password, password_verify);
        strcpy(MessageToServer, DataSend);
        sleep(1); //wait for the server to sen data
        QMessageBox msgBox;
        msgBox.setWindowTitle("Sign In");
        msgBox.setText(MessageFromServer);
        if (strstr(MessageFromServer, "User already exists\n")) {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setStyleSheet("QMessageBox{ background-color: #FAD02E; }"
                                 "QMessageBox QLabel{ color: #333; }");
            msgBox.exec();
        } else {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStyleSheet("QMessageBox{ background-color: #4CAF50; }"
                                 "QMessageBox QLabel{ color: #fff; }");
            msgBox.exec();
            this->close();
        }

    }
}

