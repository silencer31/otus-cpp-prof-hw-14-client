#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "Network/requestmanager.h"
#include "Parser/replyparser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AdminWindow; }
QT_END_NAMESPACE

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    AdminWindow() = delete;

    explicit AdminWindow(const req_mngr_shared rm_ptr, const collector_shared cltr_ptr, const parser_shared par_ptr,
                         const message_win_shared mw_ptr, const QString& uname, QWidget *parent = nullptr);

    ~AdminWindow();

private: // methods
    void check_server();

private: // data
    Ui::AdminWindow *ui;

    const req_mngr_shared request_manager_ptr;
    const collector_shared collector_ptr;
    const parser_shared parser_ptr;

    const message_win_shared message_window_ptr;

    const QString user_name;
    const int user_id;

    std::string server_reply;
};

using admin_win_unique = QScopedPointer<AdminWindow>;
#endif // ADMINWINDOW_H
