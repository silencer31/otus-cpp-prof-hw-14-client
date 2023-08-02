#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "Network/requestmanager.h"

namespace Ui {
class UserWindow;
}

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(const req_mngr_shared rm_ptr, QWidget *parent = nullptr);
    ~UserWindow();

    void set_message_window(const QSharedPointer<MessageWindow> mwp) {
        message_win_ptr = mwp;
    }

private:
    Ui::UserWindow *ui;

    req_mngr_shared request_manager_ptr;
    QSharedPointer<MessageWindow> message_win_ptr;
};

#endif // USERWINDOW_H
