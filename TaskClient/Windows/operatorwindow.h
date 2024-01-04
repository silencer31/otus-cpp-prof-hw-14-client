#ifndef OPERATORWINDOW_H
#define OPERATORWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "simple_delegate.h"

#include "Data/data_keeper.h"
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

    explicit OperatorWindow(const req_mngr_shared rm_ptr, const collector_shared cltr_ptr,
                            const parser_shared par_ptr, const data_keeper_shared dk_ptr,
                            const message_win_shared mw_ptr, const QString& uname, QWidget *parent = nullptr);

    ~OperatorWindow();

private: // methods
    void lock_buttons();

    void unlock_buttons();

    bool handle_request(CommandType comm_type);

private slots:
    void get_tasks_list();

    void appoint_user();

    void set_task_status();

    void delete_task();

    void set_deadline();

    void create_task();

private: // data
    Ui::OperatorWindow *ui;

    const req_mngr_shared request_manager_ptr;
    const collector_shared collector_ptr;
    const parser_shared parser_ptr;
    const data_keeper_shared data_keeper_ptr;

    const message_win_shared message_window_ptr;

    const QString user_name;
    const int user_id;

    // Модели данных для таблиц
    QStandardItemModel *users_table_model; // Модель для данных о пользователях.
    QStandardItemModel *tasks_table_model; // Модель для данных о задачах.

    // Делегаты для таблиц.
    SimpleItemDelegate *users_table_delegate{nullptr};
    SimpleItemDelegate *tasks_table_delegate{nullptr};

    std::string server_reply;
    QString error_text;
};

using operator_win_unique = QScopedPointer<OperatorWindow>;

#endif // OPERATORWINDOW_H
