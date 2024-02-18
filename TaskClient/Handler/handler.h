#ifndef HANDLER_H
#define HANDLER_H

#pragma once

#include <memory>

#include "Network/requestmanager.h"
#include "Parser/replyparser.h"

/***********
 *
 *  Этот класс помогает отправлять запросы и обрабатывать ответы от сервера.
 *
 * ********/

class Handler
{
public:
    Handler() = delete;

    explicit Handler(const req_mngr_shared rm_ptr, const collector_shared cltr_ptr, const parser_shared par_ptr)
        : request_manager_ptr(rm_ptr)
        , collector_ptr(cltr_ptr)
        , parser_ptr(par_ptr)
    {}

    ~Handler() = default;

    const QString& get_error() {
        return error_text;
    }

    // Обработка реакции сервера на сетевой запрос.
    bool handle_request(CommandType comm_type);

    // Тестовый запрос серверу.
    bool test_request();

    // Завершить текущую сессию.
    bool closedown_session();

    // Выключить сервер.
    bool shutdown_server();

    // Запрос возможных типов пользователей.
    bool get_user_types();

    // Запрос возможных статусов задач.
    bool get_task_statuses();

    // Логин пользователя на сервере.
    bool login_on_server(const QString& user_name, const QString& password);

    // Запрос списка пользователей.
    bool get_users_list();

    // Получить логин и тип пользователя по user id.
    bool get_username(const int id);

    // Получить ФИО пользователя по user id.
    bool get_fullname(const int id);

    // Запрос списка задач.
    bool get_taskslist(const int user_id = -1);

    // Получить данные задачи по task id.
    bool get_taskdata(const int id);

    // Создать нового пользователя.
    bool create_user(const QString& user_name, int user_type, const QString& pass,
                     const QString& second, const QString& first, const QString& patron);

    // Удалить пользователя.
    bool delete_user(const int user_id);

    // Изменить тип пользователя.
    bool change_usertype(const int user_id, const int user_type);

    // Установить новый пароль пользователю.
    bool set_password(const int user_id, const QString& password);

    // Создать новую задачу.
    bool create_task(const int user_id, const QString& deadline, const QString& name, const QString& description);

    // Удалить задачу по её id.
    bool delete_task(const int task_id);

    // Изменить статус задачи.
    bool change_task_status(const int task_id, const int status);

    // Изменить deadline задачи.
    bool set_task_deadline(const int task_id, const QString& deadline);

    // Назначить пользователя на задачу.
    bool appoint_task_user(const int task_id, const int user_id);

private:
    const req_mngr_shared request_manager_ptr;
    const collector_shared collector_ptr;
    const parser_shared parser_ptr;

    std::string server_reply;
    QString error_text;
};

using handler_shared = std::shared_ptr<Handler>;

#endif // HANDLER_H
