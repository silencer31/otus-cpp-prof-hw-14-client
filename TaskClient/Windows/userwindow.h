#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "simple_delegate.h"

#include "Data/data_keeper.h"
#include "Network/requestmanager.h"
#include "Parser/replyparser.h"

namespace Ui {
class UserWindow;
}

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(const req_mngr_shared rm_ptr, const collector_shared cltr_ptr,
                        const parser_shared par_ptr, const data_keeper_shared dk_ptr,
                        const message_win_shared mw_ptr, const QString& uname, QWidget *parent = nullptr);

    ~UserWindow();

private: // methods
    void lock_buttons();

    void unlock_buttons();

    bool handle_request(CommandType comm_type);

private slots:
    void get_tasks_list();

    void take_task();

    void set_task_status();

    void create_task();

private: // data
    Ui::UserWindow *ui;

    const req_mngr_shared request_manager_ptr;
    const collector_shared collector_ptr;
    const parser_shared parser_ptr;
    const data_keeper_shared data_keeper_ptr;

    const message_win_shared message_window_ptr;

    const QString user_name;
    const int user_id;

    QStandardItemModel *tasks_table_model; // Модель для таблицы с данными о задачах.

    SimpleItemDelegate *tasks_table_delegate{nullptr}; // Делегат для таблицы.

    std::string server_reply;
    QString error_text;
};

using user_win_unique = QScopedPointer<UserWindow>;

#endif // USERWINDOW_H
