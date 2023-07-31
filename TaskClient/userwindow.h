#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "requestmanager.h"

namespace Ui {
class UserWindow;
}

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(const request_manager_ptr rm_ptr, QWidget *parent = nullptr);
    ~UserWindow();

    void set_message_window(const QSharedPointer<MessageWindow> mwp) {
        message_win_ptr = mwp;
    }

private:
    Ui::UserWindow *ui;

    request_manager_ptr req_man_ptr;
    QSharedPointer<MessageWindow> message_win_ptr;
};

#endif // USERWINDOW_H
