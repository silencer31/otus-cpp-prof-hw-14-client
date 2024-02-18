#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "passwdwindow.h"
#include "simple_delegate.h"

#include "Handler/handler.h"
#include "Data/data_keeper.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AdminWindow; }
QT_END_NAMESPACE

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    AdminWindow() = delete;

    explicit AdminWindow(const req_mngr_shared rm_ptr, const collector_shared cltr_ptr, const parser_shared par_ptr,
                         const handler_shared hdlr_ptr,const data_keeper_shared dk_ptr,
                         const message_win_shared mw_ptr, const passwd_win_shared pwd_ptr,
                         QWidget *parent = nullptr);

    ~AdminWindow();

    void output_user_data(); // Вывести данные текущего пользователя.

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent*);

private: // methods
    void lock_buttons();

    void unlock_buttons();

    // Показать сообщение пользователю.
    void show_message(const QString& message);

    // Проверка связи с сервером.
    void check_server();

    // Запрос на создание нового пользователя.
    void create_user();

    // Удалить выбранного пользователя.
    void delete_user();

    // Запрос на изменение типа пользователя.
    void change_user_type();

    // Запрос на изменение пароля.
    void change_password();

private slots:
    // Реакция на клик по строке в таблице с пользователями.
    void user_clicked(const QModelIndex& index);

    // Реакция на клик по строке в таблице с задачами.
    void task_clicked(const QModelIndex& index);

    // Запрос списка пользователей.
    void get_users_list();

    // Запрос списка задач.
    void get_tasks_list();

    // Создать нового или изменить данные выбранного пользователя.
    void add_or_edit_user();

    // Очистить поля ввода.
    void clear_fields();

    // Отправить запрос на выключение сервера.
    void shutdown_server();

private: // data
    Ui::AdminWindow *ui;

    const req_mngr_shared request_manager_ptr;
    const collector_shared collector_ptr;
    const parser_shared parser_ptr;
    const handler_shared handler_ptr;  // Обработчик ответов от сервера на запросы.
    const data_keeper_shared data_keeper_ptr;

    const message_win_shared message_window_ptr;
    const passwd_win_shared passwd_window_ptr;

    // Модели данных для таблиц
    QStandardItemModel *users_table_model; // Модель для данных о пользователях.
    QStandardItemModel *tasks_table_model; // Модель для данных о задачах.

    // Делегаты для таблиц.
    SimpleItemDelegate *users_table_delegate{nullptr};
    SimpleItemDelegate *tasks_table_delegate{nullptr};
};

using admin_win_unique = QScopedPointer<AdminWindow>;
#endif // ADMINWINDOW_H
