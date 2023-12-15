#ifndef OPERATORWINDOW_H
#define OPERATORWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "Network/requestmanager.h"
#include "Parser/replyparser.h"

namespace Ui {
class OperatorWindow;
}

class OperatorWindow : public QMainWindow
{
    Q_OBJECT

public:
    OperatorWindow() = delete;

    explicit OperatorWindow(const req_mngr_shared rm_ptr, const collector_shared cltr_ptr, const parser_shared par_ptr,
                            const message_win_shared mw_ptr, const QString& uname, QWidget *parent = nullptr);

    ~OperatorWindow();

private:
    Ui::OperatorWindow *ui;

    const req_mngr_shared request_manager_ptr;
    const collector_shared collector_ptr;
    const parser_shared parser_ptr;

    const message_win_shared message_window_ptr;

    const QString user_name;
    const int user_id;

    std::string server_reply;
};

using operator_win_unique = QScopedPointer<OperatorWindow>;

#endif // OPERATORWINDOW_H
