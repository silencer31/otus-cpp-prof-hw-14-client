#ifndef OPERATORWINDOW_H
#define OPERATORWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "requestmanager.h"

namespace Ui {
class OperatorWindow;
}

class OperatorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OperatorWindow(const request_manager_ptr rm_ptr, QWidget *parent = nullptr);
    ~OperatorWindow();

    void set_message_window(const QSharedPointer<MessageWindow> mwp) {
        message_win_ptr = mwp;
    }

private:
    Ui::OperatorWindow *ui;

    request_manager_ptr req_man_ptr;
    QSharedPointer<MessageWindow> message_win_ptr;
};

#endif // OPERATORWINDOW_H
