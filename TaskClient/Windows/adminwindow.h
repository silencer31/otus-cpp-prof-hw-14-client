#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "Network/requestmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AdminWindow; }
QT_END_NAMESPACE

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    AdminWindow(const req_mngr_shared rm_ptr, const message_win_shared mw_ptr, QWidget *parent = nullptr);

    ~AdminWindow();

private:
    Ui::AdminWindow *ui;

    const req_mngr_shared request_manager_ptr;
    const message_win_shared message_window_ptr;
};

using admin_win_unique = QScopedPointer<AdminWindow>;
#endif // ADMINWINDOW_H
