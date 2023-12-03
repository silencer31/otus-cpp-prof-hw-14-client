#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#pragma once

#include <memory>

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

    bool connected_to_server() const {
        return net_communication_ptr->is_connected();
    }

    const std::string& get_last_error() const {
        return net_communication_ptr->get_last_error();
    }

    void disconnect_from_server() {
        net_communication_ptr->disconnect();
    }

    bool get_server_answer(std::string& answer);

    void reset_request() {
        current_request = CommandType::Unknown;
    }

    bool send_test_request();

    bool send_login(const std::string& login, const std::string& password);

    bool send_closedown();

    bool send_shutdown();



private:
    const net_comm_unique net_communication_ptr;

    CommandType current_request;
};

using req_mngr_shared = std::shared_ptr<RequestManager>;

#endif // REQUESTMANAGER_H
