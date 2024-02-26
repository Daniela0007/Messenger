#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "registerdialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_SIGN_UP_BUTTON_clicked()
{
    RegisterDialog RegisterWindow; //la clickul de Login button, ne directioneaza catre o noua fereastra
    RegisterWindow.setModal(true);
    RegisterWindow.exec();
    /**/
}


void MainWindow::on_SIGN_IN_BUTTON_clicked()
{
    LoginDialog LoginWindow;
    LoginWindow.setModal(true);
    LoginWindow.exec();
}

