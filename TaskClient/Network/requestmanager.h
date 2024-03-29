#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#pragma once

#include <memory>
#include<QString>

#include "netcommunication.h"
#include "Data/data_types.h"

/***********
 *
 *  Этот класс формирует запросы к серверу.
 *
 * ********/

class RequestManager
{
public:
    RequestManager() = delete;

    explicit RequestManager(const std::string& addr, const int port)
        : net_communication_ptr(std::make_unique<NetCommunication>(addr, port))
        , current_request(CommandType::Unknown)
    {}

    bool reconnect_to_server() {
        return (net_communication_ptr->is_connected() ? true : net_communication_ptr->reconnect());
    }

    bool connected_to_server() const {
        return net_communication_ptr->is_connected();
    }

    QString get_last_error() const {
        return QString::fromStdString(net_communication_ptr->get_last_error());
    }

    void disconnect_from_server() {
        net_communication_ptr->disconnect();
    }

    void reset_request() {
        current_request = CommandType::Unknown;
    }

    bool get_server_answer(std::string& answer);

    bool send_test_request();

    bool send_get_user_types(); // Получить список возможных типов пользователя и список с их описанием.

    bool send_get_task_statuses(); // Получить список возможных статусов задачи и список с их описанием.

    bool send_login(const QString& user_name, const QString& password);

    bool send_closedown();

    bool send_shutdown();

    bool send_get_username(const int id); // Получить логин и тип пользователя по user id.

    bool send_get_fullname(const int id); // Получить ФИО пользователя по user id.

    bool send_get_userlist(); // Получить список всех id пользователей.

    bool send_get_tasklist(const int id = -1); // Если указан user_id, получить список его задач, а если не указан, то список всех задач.

    bool send_get_taskdata(const int id); // Получить данные задачи по task id.

    bool send_add_user(const QString& user_name, const int user_type, const QString& pass,
                       const QString& second, const QString& first, const QString& patron);

    bool send_add_task(const int user_id, const QString& deadline, const QString& name, const QString& description);

    bool send_del_user(const int id);

    bool send_del_task(const int id);

    bool send_set_password(const int user_id, const QString& password);

    bool send_set_usertype(const int user_id, const int user_type);

    bool send_set_taskstatus(const int task_id, const int status);

    bool send_set_taskuser(const int task_id, const int user_id);

    bool send_set_deadline(const int task_id, const QString& deadline);

private:
    const net_comm_unique net_communication_ptr;

    CommandType current_request;
};

using req_mngr_shared = std::shared_ptr<RequestManager>;

#endif // REQUESTMANAGER_H
