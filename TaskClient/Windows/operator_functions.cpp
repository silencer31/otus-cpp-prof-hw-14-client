#include "operatorwindow.h"
#include "ui_operatorwindow.h"

// Запрос возможных типов пользователей и статусов задач.
bool OperatorWindow::get_user_types()
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

    return true;
}

// Запрос возможных статусов задач.
bool OperatorWindow::get_task_statuses()
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
    if (!collector_ptr->task_statuses_received()) {
        message_window_ptr->set_message(QString("Получена пустая коллекция статусов задач"));
        message_window_ptr->exec();
        return false;
    }

    // Обновляем список доступных статусов в выпадающем меню.
    ui->cbTaskStatus->clear();

    for(auto iter = collector_ptr->task_statuses_cib(); iter != collector_ptr->task_statuses_cie(); ++iter) {
        ui->cbTaskStatus->addItem(QString("%1 : %2").arg(QString::number(iter.key()), iter.value()));
    }

    return true;
}

// Создать новую задачу.
void OperatorWindow::create_task()
{
    const QString task_name   = ui->leTaskName->text();
    const QString deadline    = ui->leDeadLine->text();
    const QString description = ui->leDescription->text();

    // Проверка, заполнены ли поля.
    if (task_name.isEmpty() || deadline.isEmpty() || description.isEmpty()) {
        message_window_ptr->set_message(QString("All the fields have to be filled in!"));
        message_window_ptr->exec();
        return;
    }

    // Проверка, используется ли уже такое имя задачи.
    if (data_keeper_ptr->task_name_received(task_name)) {
        message_window_ptr->set_message(QString("Task name %1 is already in use!\nPlease enter another value").arg(task_name));
        message_window_ptr->exec();
        return;
    }
}

// Удалить выбранную задачу.
void OperatorWindow::delete_task()
{

}

// Изменить статус задачи.
void OperatorWindow::change_task_status()
{

}

// Задать новую дату, до которой д/б выполнена задача.
void OperatorWindow::set_task_deadline()
{

}

// Назначить другого пользователя на задачу.
void OperatorWindow::appoint_user()
{

}
