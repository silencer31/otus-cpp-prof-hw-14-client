#include "requestmanager.h"

RequestManager::RequestManager(const net_communication_ptr nc_ptr) :
    net_comm_ptr(nc_ptr)
{

}

bool RequestManager::login_on_server(const std::string& login, const std::string& password)
{
    return true;
}
