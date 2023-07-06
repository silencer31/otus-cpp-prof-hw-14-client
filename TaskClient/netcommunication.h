#ifndef NETCOMMUNICATION_H
#define NETCOMMUNICATION_H

#include <boost/asio.hpp>

namespace ba = boost::asio;

class NetCommunication
{
public:
    explicit NetCommunication(const std::string& ip_addr, int port);

    bool connect_to_server();

    bool send_message(const std::string& text);

private:
    std::string last_error;

    ba::ip::tcp::endpoint _end_point;

    ba::io_context _io_context;
    ba::ip::tcp::socket _sock;
};

#endif // NETCOMMUNICATION_H
