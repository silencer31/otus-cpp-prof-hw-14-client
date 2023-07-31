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

    explicit RequestManager(const net_communication_ptr nc_ptr);

    bool login_on_server(const std::string& login, const std::string& password);

private:
    net_communication_ptr net_comm_ptr;
};

using request_manager_ptr = std::shared_ptr<RequestManager>;

#endif // REQUESTMANAGER_H
