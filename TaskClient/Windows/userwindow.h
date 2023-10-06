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
    explicit UserWindow(const req_mngr_shared rm_ptr, const message_win_shared mw_ptr, QWidget *parent = nullptr);
    ~UserWindow();

private:
    Ui::UserWindow *ui;

    const req_mngr_shared request_manager_ptr;
    const message_win_shared message_window_ptr;
};

using user_win_unique = QScopedPointer<UserWindow>;

#endif // USERWINDOW_H
