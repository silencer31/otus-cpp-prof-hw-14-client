#ifndef NETCOMMUNICATION_H
#define NETCOMMUNICATION_H

#include <boost/asio.hpp>

namespace ba = boost::asio;

class NetCommunication
{
public:
    NetCommunication();

    bool connect_to_server();

    bool send_message(const std::string& text);

private:
    std::string last_error;

    ba::io_context _io_context;
    ba::ip::tcp::endpoint _ep = ba::ip::tcp::endpoint(ba::ip::address::from_string("127.0.0.1"), 2023);

    ba::ip::tcp::socket _sock = ba::ip::tcp::socket(_io_context);
};

#endif // NETCOMMUNICATION_H
