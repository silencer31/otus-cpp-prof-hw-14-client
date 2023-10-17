#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#pragma once

#include "netcommunication.h"

/***********
 *
 *  Этот класс формирует запросы к серверу.
 *
 * ********/

class RequestManager
{
public:
    RequestManager() = delete;

    explicit RequestManager(const net_comm_shared nc_ptr)
        : net_communication_ptr(nc_ptr)
    {}

    bool send_test();

    bool login_on_server(const std::string& login, const std::string& password);

    bool shutdown_server(const int user_id, const std::string& password);

    bool closedown_session();

private:
    const net_comm_shared net_communication_ptr;
};

using req_mngr_shared = std::shared_ptr<RequestManager>;

#endif // REQUESTMANAGER_H
