#ifndef HANDLER_H
#define HANDLER_H

#pragma once

#include <memory>

#include "Network/requestmanager.h"
#include "Parser/replyparser.h"

/***********
 *
 *  Этот класс помогает обрабатывать ответ от сервера на запросы.
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

    // Обработка реакции сервера на сетевой запрос.
    bool handle_request(CommandType comm_type);

    // Запрос возможных типов пользователей.
    bool get_user_types();

    // Запрос возможных статусов задач.
    bool get_task_statuses();

    // Логин пользователя на сервере.
    bool login_on_server(const QString& user_name, const QString& password);

    const QString& get_error() {
        return error_text;
    }

private:
    const req_mngr_shared request_manager_ptr;
    const collector_shared collector_ptr;
    const parser_shared parser_ptr;

    std::string server_reply;
    QString error_text;
};

using handler_shared = std::shared_ptr<Handler>;

#endif // HANDLER_H
