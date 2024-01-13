#ifndef OPERATORWINDOW_H
#define OPERATORWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "passwdwindow.h"
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
                            const message_win_shared mw_ptr, const passwd_win_shared pwd_ptr,
                            const QString& uname, QWidget *parent = nullptr);

    ~OperatorWindow();

private: // methods
    void lock_buttons();

    void unlock_buttons();

    bool handle_request(CommandType comm_type);

    // Запрос возможных типов пользователей.
    bool get_user_types();

    // Запрос возможных статусов задач.
    bool get_task_statuses();

    void create_task(); // Создать новую задачу.

    void delete_task(); // Удалить выбранную задачу.

    void change_task_status(); // Изменить статус задачи.

    void set_task_deadline(); // Задать новую дату, до которой д/б выполнена задача.

    void appoint_user(); // Назначить другого пользователя на задачу.

private slots:
    void user_clicked(const QModelIndex&); // Реакция на клик по строке в таблице с пользователями.

    void task_clicked(const QModelIndex&); // Реакция на клик по строке в таблице с задачами.

    void get_users_list(); // Запрос списка пользователей.

    void get_tasks_list(); // Запрос списка задач.

    void add_or_edit_task(); // Создать новую или изменить данные выбранной задачи.

    void clear_fields(); // Очистить поля ввода.


private: // data
    Ui::OperatorWindow *ui;

    const req_mngr_shared request_manager_ptr;
    const collector_shared collector_ptr;
    const parser_shared parser_ptr;
    const data_keeper_shared data_keeper_ptr;

    const message_win_shared message_window_ptr;
    const passwd_win_shared passwd_window_ptr;

    const QString own_name;
    const int own_id;

    // Модели данных для таблиц
    QStandardItemModel *users_table_model; // Модель для данных о пользователях.
    QStandardItemModel *tasks_table_model; // Модель для данных о задачах.

    // Делегаты для таблиц.
    SimpleItemDelegate *users_table_delegate{nullptr};
    SimpleItemDelegate *tasks_table_delegate{nullptr};

    QMap<int, int> index_y_task_id_map; // Для связи между строкой таблицы и id задачи.
    QMap<int, int> index_y_user_id_map; // Для связи между строкой таблицы и id пользователя.


    std::string server_reply;
    QString error_text;
};

using operator_win_unique = QScopedPointer<OperatorWindow>;

#endif // OPERATORWINDOW_H
