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
    explicit OperatorWindow(const req_mngr_shared rm_ptr, QWidget *parent = nullptr);
    ~OperatorWindow();

    void set_message_window(const QSharedPointer<MessageWindow> mwp) {
        message_win_ptr = mwp;
    }

private:
    Ui::OperatorWindow *ui;

    req_mngr_shared request_manager_ptr;
    QSharedPointer<MessageWindow> message_win_ptr;
};

#endif // OPERATORWINDOW_H
