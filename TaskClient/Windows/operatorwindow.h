#ifndef OPERATORWINDOW_H
#define OPERATORWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "Network/requestmanager.h"

namespace Ui {
class OperatorWindow;
}

class OperatorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OperatorWindow(const req_mngr_shared rm_ptr, const message_win_shared mw_ptr, QWidget *parent = nullptr);
    ~OperatorWindow();

private:
    Ui::OperatorWindow *ui;

    const req_mngr_shared request_manager_ptr;
    const message_win_shared message_window_ptr;
};

using operator_win_unique = QScopedPointer<OperatorWindow>;

#endif // OPERATORWINDOW_H
