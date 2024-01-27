#include "userwindow.h"
#include "ui_userwindow.h"

// Запрос возможных типов пользователей и статусов задач.
bool UserWindow::get_user_types()
{
    // Проверяем, был ли ранее получен список типов пользователей.
    if (collector_ptr->user_types_received()) {
        return true;
    }

    // Запрос списка типа пользователей.
    if (!request_manager_ptr->send_get_typelist()) {
        show_message(QString("Unable to send get typelist request!\n\n%1")
                                    .arg(request_manager_ptr->get_last_error()));
        return false;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Get)) {
        show_message(QString("Unable to get user types!\n\n%1").arg(error_text));
        return false;
    }

    // Проверяем, что получено от сервера.
    if (!collector_ptr->user_types_received()) {
        show_message(QString("Получена пустая коллекция типов пользователей"));
        return false;
    }

    return true;
}

// Запрос возможных статусов задач.
bool UserWindow::get_task_statuses()
{
    // Проверяем, был ли ранее получен список статусов задач.
    if (collector_ptr->task_statuses_received()) {
        return true;
    }

    // Запрос списка статусов задач.
    if (!request_manager_ptr->send_get_statuslist()) {
        show_message(QString("Unable to send get statuslist request!\n\n%1")
                                .arg(request_manager_ptr->get_last_error()));
        return false;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Get)) {
        show_message(QString("Unable to get task statuses!\n\n%1").arg(error_text));
        return false;
    }

    // Проверяем, что получено от сервера.
    if (!collector_ptr->task_statuses_received()) {
        show_message(QString("Получена пустая коллекция статусов задач"));
        return false;
    }

    // Обновляем список доступных статусов в выпадающем меню.
    ui->cbTaskStatus->clear();

    for(auto iter = collector_ptr->task_statuses_cib(); iter != collector_ptr->task_statuses_cie(); ++iter) {
        ui->cbTaskStatus->addItem(QString("%1 : %2").arg(QString::number(iter.key()), iter.value()));
    }

    return true;
}

// Создать задачу для себя.
void UserWindow::create_task()
{

}

// Забрать чужую задачу себе.
void UserWindow::take_task()
{

}

// Изменить статус своей задачи.
void UserWindow::change_task_status()
{

}

// Задать новую дату, до которой д/б выполнена своя задача.
void UserWindow::set_task_deadline()
{

}
