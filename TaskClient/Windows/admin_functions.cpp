#include "adminwindow.h"
#include "ui_adminwindow.h"

// Запрос возможных типов пользователей и статусов задач.
bool AdminWindow::get_user_types()
{
    // Проверяем, был ли ранее получен список типов пользователей.
    if (collector_ptr->user_types_received()) {
        return true;
    }

    // Запрос списка типа пользователей.
    if (!request_manager_ptr->send_get_typelist()) {
        message_window_ptr->set_message(QString("Unable to send get typelist request!\n\n%1")
                                            .arg(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return false;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Get)) {
        message_window_ptr->set_message(QString("Unable to get user types!\n\n%1").arg(error_text));
        message_window_ptr->exec();
        return false;
    }

    // Проверяем, что получено от сервера.
    if (!collector_ptr->user_types_received()) {
        message_window_ptr->set_message(QString("Получена пустая коллекция типов пользователей"));
        message_window_ptr->exec();
        return false;
    }


    // Обновляем список доступных типов в выпадающем меню.
    ui->cbUserType->clear();

    for(auto iter = collector_ptr->user_types_cib(); iter != collector_ptr->user_types_cie(); ++iter) {
        ui->cbUserType->addItem(QString("%1 : %2").arg(QString::number(iter.key()), iter.value()));
    }

    return true;
}

// Запрос возможных статусов задач.
bool AdminWindow::get_task_statuses()
{
    // Проверяем, был ли ранее получен список статусов задач.
    if (collector_ptr->task_statuses_received()) {
        return true;
    }

    // Запрос списка статусов задач.
    if (!request_manager_ptr->send_get_statuslist()) {
        message_window_ptr->set_message(QString("Unable to send get statuslist request!\n\n%1")
                                            .arg(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return false;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Get)) {
        message_window_ptr->set_message(QString("Unable to get task statuses!\n\n%1").arg(error_text));
        message_window_ptr->exec();
        return false;
    }

    // Проверяем, что получено от сервера.
    if (collector_ptr->task_statuses_received()) {
        return true;
    }

    message_window_ptr->set_message(QString("Получена пустая коллекция статусов задач"));
    message_window_ptr->exec();
    return false;
}

// Запрос на создание нового пользователя.
void AdminWindow::create_user()
{

}

// Запрос на изменение данных пользователя.
void AdminWindow::change_user_data()
{

}

// Запрос на изменение пароля.
void AdminWindow::change_password()
{

}
