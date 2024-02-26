#ifndef USERSLISTDIALOG_H
#define USERSLISTDIALOG_H

#include <QDialog>

namespace Ui {
class UsersListDialog;
}

class UsersListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UsersListDialog(QWidget *parent = nullptr);
    ~UsersListDialog();

private:
    Ui::UsersListDialog *ui;
private slots:
    void GO_TO_CONVERSATION(const QString &textButton);
    void on_EXIT_BUTTON_clicked();
};

#endif // USERSLISTDIALOG_H
