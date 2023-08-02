#include "requestmanager.h"

RequestManager::RequestManager(const net_comm_shared nc_ptr) :
    net_communication_ptr(nc_ptr)
{

}

bool RequestManager::login_on_server(const std::string& login, const std::string& password)
{
    return true;
}
