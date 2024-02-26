#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>

namespace Ui {
class SignDialog;
}

class SignDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignDialog(QWidget *parent = nullptr);
    ~SignDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SignDialog *ui;
};

#endif // SIGNUPDIALOG_H
