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

// Тестовый запрос серверу.
bool Handler::test_request()
{
    // Отправляем тестовый запрос, чтобы узнать, готов ли сервер обрабатывать запросы.
    if (!request_manager_ptr->send_test_request()) {
        error_text = QString("Unable to send test request!\n%1").arg(request_manager_ptr->get_last_error());
        return false;
    }

    // Контроль выполнения запроса.
    return handle_request(CommandType::Test);
}

// Выключить сервер.
bool Handler::shutdown_server()
{
    // Отправляем запрос на сервер.
    if ( !request_manager_ptr->send_shutdown()) {
        error_text = QString("Unable to send shutdown reques\n%1").arg(request_manager_ptr->get_last_error());
        return false;
    }

    // Контроль выполнения запроса.
    return handle_request(CommandType::Shutdown);
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

    // Контроль выполнения запроса.
    return handle_request(CommandType::Login);
}

// Запрос списка пользователей.
bool Handler::get_users_list()
{
    // Отправляем запрос на сервер.
    if (!request_manager_ptr->send_get_userlist()) {
        error_text = QString("Unable to send get userlist request!\n%1").arg(request_manager_ptr->get_last_error());
        return false;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Get)) {
        return false;
    }

    // Проверяем, что получено от сервера.
    if (!collector_ptr->user_ids_received()) {
        error_text = QString("Empty user ids collection has been received");
        return false;
    }

    return true;
}

// Получить логин и тип пользователя по user id.
bool Handler::get_username(const int id)
{
    // Запрос логина и типа пользователя.
    if (!request_manager_ptr->send_get_username(id)) {
        error_text = QString("Unable to send get username request!\n%1").arg(request_manager_ptr->get_last_error());
        return false;
    }

    // Контроль выполнения запроса.
    return handle_request(CommandType::Get);
}

// Получить ФИО пользователя по user id.
bool Handler::get_fullname(const int id)
{
    // Запрос полного имени пользователя.
    if (!request_manager_ptr->send_get_fullname(id)) {
        error_text = QString("Unable to send get fullname request!\n%1").arg(request_manager_ptr->get_last_error());
        return false;
    }

    // Контроль выполнения запроса.
    return handle_request(CommandType::Get);
}

// Запрос списка задач.
bool Handler::get_taskslist(const int user_id)
{
    // Запрос полного имени пользователя.
    if (!request_manager_ptr->send_get_tasklist(user_id)) {
        error_text = QString("Unable to send get tasklist request!\n%1").arg(request_manager_ptr->get_last_error());
        return false;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Get)) {
        return false;
    }

    // Проверяем, что получено от сервера.
    if (!collector_ptr->task_ids_received()) {
        error_text = QString("Empty task ids collection has been received");
        return false;
    }

    return true;
}

// Получить данные задачи по task id.
bool Handler::get_taskdata(const int id)
{
    // Запрос данных одной задачи.
    if (!request_manager_ptr->send_get_taskdata(id)) {
        error_text = QString("Unable to send get taskdata request\n%1").arg(request_manager_ptr->get_last_error());
        return false;
    }

    // Контроль выполнения запроса.
    return handle_request(CommandType::Get);
}

// Создать нового пользователя.
bool Handler::create_user(const QString& user_name, int user_type, const QString& pass,
                 const QString& second, const QString& first, const QString& patron)
{
    // Отправляем запрос на создание нового пользователя.
    if ( !request_manager_ptr->send_add_user(user_name, user_type, pass, second, first, patron) ) {
        error_text = QString("Unable to send add user request\n%1").arg(request_manager_ptr->get_last_error());
        return false;
    }

    // Контроль выполнения запроса.
    return handle_request(CommandType::Add);
}

// Удалить пользователя.
bool Handler::delete_user(const int user_id)
{
    if ( !request_manager_ptr->send_del_user(user_id) ) {
        error_text = QString("Unable to send del user request\n%1").arg(request_manager_ptr->get_last_error());
        return false;
    }

    // Контроль выполнения запроса.
    return handle_request(CommandType::Del);
}
