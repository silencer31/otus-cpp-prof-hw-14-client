#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "requestmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AdminWindow; }
QT_END_NAMESPACE

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    AdminWindow(const request_manager_ptr rm_ptr, QWidget *parent = nullptr);

    ~AdminWindow();

    void set_message_window(const QSharedPointer<MessageWindow> mwp) {
        message_win_ptr = mwp;
    }

private:
    Ui::AdminWindow *ui;

    request_manager_ptr req_man_ptr;
    QSharedPointer<MessageWindow> message_win_ptr;
};
#endif // ADMINWINDOW_H
