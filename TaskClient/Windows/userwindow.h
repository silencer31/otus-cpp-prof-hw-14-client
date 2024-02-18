#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "passwdwindow.h"
#include "simple_delegate.h"

#include "Handler/handler.h"
#include "Data/data_keeper.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class UserWindow;
}
QT_END_NAMESPACE

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(const req_mngr_shared rm_ptr, const collector_shared cltr_ptr, const parser_shared par_ptr,
                        const handler_shared hdlr_ptr, const data_keeper_shared dk_ptr,
                        const message_win_shared mw_ptr, const passwd_win_shared pwd_ptr,
                        QWidget *parent = nullptr);

    ~UserWindow();

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

    void create_task(); // Создать задачу для себя.

    void take_task(); // Забрать чужую задачу себе.

    void change_task_status(); // Изменить статус своей задачи.

    void set_task_deadline(); // Задать новую дату, до которой д/б выполнена своя задача.

private slots:
    // Реакция на клик по строке в таблице с задачами.
    void task_clicked(const QModelIndex&);

    // Запрос списка задач.
    void get_tasks_list();

    // Создать новую или изменить данные выбранной задачи.
    void add_or_edit_task();

    // Очистить поля ввода.
    void clear_fields();

private: // data
    Ui::UserWindow *ui;

    const req_mngr_shared request_manager_ptr;
    const collector_shared collector_ptr;
    const parser_shared parser_ptr;
    const handler_shared handler_ptr; // Обработчик ответов от сервера на запросы.
    const data_keeper_shared data_keeper_ptr;

    const message_win_shared message_window_ptr;
    const passwd_win_shared passwd_window_ptr;

    QStandardItemModel *tasks_table_model; // Модель для таблицы с данными о задачах.

    SimpleItemDelegate *tasks_table_delegate{nullptr}; // Делегат для таблицы.
};

using user_win_unique = QScopedPointer<UserWindow>;

#endif // USERWINDOW_H
