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

    explicit RequestManager(const net_comm_shared nc_ptr);

    bool login_on_server(const std::string& login, const std::string& password);

private:
    net_comm_shared net_comm_ptr;
};

using req_mngr_shared = std::shared_ptr<RequestManager>;

#endif // REQUESTMANAGER_H
