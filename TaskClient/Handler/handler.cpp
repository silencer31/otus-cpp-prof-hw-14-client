#include "handler.h"

// Обработка реакции сервера на сетевой запрос.
bool Handler::handle_request(CommandType comm_type)
{
    // Пытаемся получить ответ от сервера.
    if (!request_manager_ptr->get_server_answer(server_reply)) {
        error_text = QString("Server reply read error: %1").arg(request_manager_ptr->get_last_error());
        return false;
    }

    // Пробуем распарсить json в ответе.
    if ( !parser_ptr->handle_reply(comm_type , server_reply)) {
        error_text = QString("Parse server reply error: %1").arg(parser_ptr->get_last_error());
        return false;
    }

    // Проверка наличия ошибки в запросе
    if (collector_ptr->req_error()) {
        error_text = QString("Unable to perform request: %1").arg(collector_ptr->get_error_text());
        return false;
    }

    // Проверка результата выполнения запроса.
    if (!collector_ptr->get_result()) {
        error_text = collector_ptr->get_details();
        return false;
    }

    return true;
}

// Запрос возможных типов пользователей.
bool Handler::get_user_types()
{
    // Проверяем, был ли ранее получен список типов пользователей.
    if (collector_ptr->user_types_received()) {
        return true;
    }

    // Запрос списка типов пользователей.
    if (!request_manager_ptr->send_get_typelist()) {
        error_text = QString("Send request error: %1").arg(request_manager_ptr->get_last_error());
        return false;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Get)) {
        return false;
    }

    // Проверяем, что получено от сервера.
    if (!collector_ptr->user_types_received()) {
        error_text = QString("Empty user types collection has been received");
        return false;
    }

    return true;
}

// Запрос возможных статусов задач.
bool Handler::get_task_statuses()
{
    // Проверяем, был ли ранее получен список статусов задач.
    if (collector_ptr->task_statuses_received()) {
        return true;
    }

    // Запрос списка статусов задач.
    if (!request_manager_ptr->send_get_statuslist()) {
        error_text = QString("Send request error: %1").arg(request_manager_ptr->get_last_error());
        return false;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Get)) {
        return false;
    }

    // Проверяем, что получено от сервера.
    if (!collector_ptr->task_statuses_received()) {
        error_text = QString("Empty task statuses collection has been received");
        return false;
    }

    return true;
}

// Логин пользователя на сервере.
bool Handler::login_on_server(const QString& user_name, const QString& password)
{
    // Отправляем запрос логин на сервер.
    if ( !request_manager_ptr->send_login(user_name, password) ) {
        error_text = request_manager_ptr->get_last_error();
        return false;
    }

    return handle_request(CommandType::Login);
}
