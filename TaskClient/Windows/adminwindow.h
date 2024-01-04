#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "simple_delegate.h"

#include "Data/data_keeper.h"
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

    explicit AdminWindow(const req_mngr_shared rm_ptr, const collector_shared cltr_ptr,
                         const parser_shared par_ptr, const data_keeper_shared dk_ptr,
                         const message_win_shared mw_ptr, const QString& uname, QWidget *parent = nullptr);

    ~AdminWindow();

private: // methods
    void lock_buttons();

    void unlock_buttons();

    void check_server();

    bool handle_request(CommandType comm_type);

    // Запрос возможных типов пользователей.
    bool get_user_types();

    // Запрос возможных статусов задач.
    bool get_task_statuses();

private slots:
    void get_users_list();

    void get_tasks_list();

    void apply_changes();

    void create_user();

    void set_new_password();

    void shutdown_server();

private: // data
    Ui::AdminWindow *ui;

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

using admin_win_unique = QScopedPointer<AdminWindow>;
#endif // ADMINWINDOW_H
