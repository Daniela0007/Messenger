#include "signdialog.h"
#include "ui_signdialog.h"
#include <QMessageBox>
//#include <string.h>
//#include <stdio.h>
#include <unistd.h>
#include <utils.h>
//#include "logindialog.h"

SignDialog::SignDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignDialog)
{
    ui->setupUi(this);
}

SignDialog::~SignDialog()
{
    delete ui;
}

