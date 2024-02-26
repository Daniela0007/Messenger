#ifndef CHATWINDOWDIALOG_H
#define CHATWINDOWDIALOG_H

#include <QDialog>
#include <QString>
#include <QTreeWidgetItem>
#include <chatmanager.h>

namespace Ui {
class ChatWindowDialog;
}

class ChatWindowDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChatWindowDialog(QWidget *parent = nullptr);
    ChatManager *chatManager;

private slots:
    void on_SendButton_clicked();
    void on_MessageList_itemClicked(QTreeWidgetItem *item, int column);

    void on_BackButton_clicked();

private:
    Ui::ChatWindowDialog *ui;
public slots:
    void HandleMessageList(QString);
};

#endif // CHATWINDOWDIALOG_H
