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

    // Запрос на создание нового пользователя.
    void create_user();

    // Запрос на изменение данных пользователя.
    void change_user_data();

    // Запрос на изменение пароля.
    void change_password();

private slots:
    void user_clicked(const QModelIndex&); // Реакция на клик по строке в таблице с пользователями.

    void get_users_list(); // Запрос списка пользователей.

    void get_tasks_list(); // Запрос списка задач.

    void add_or_edit_user(); // Создать нового или изменить данные выбранного пользователя.

    void clear_fields(); // Очистить поля ввода.

    void shutdown_server(); // Отправить запрос на выключение сервера.

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

    QMap<int, int> index_y_user_id_map; // Для связи между строкой таблицы и id пользователя.


    std::string server_reply;
    QString error_text;
};

using admin_win_unique = QScopedPointer<AdminWindow>;
#endif // ADMINWINDOW_H
