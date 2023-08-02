#ifndef NETCOMMUNICATION_H
#define NETCOMMUNICATION_H

#pragma once

#include <boost/asio.hpp>

namespace ba = boost::asio;

/***********
 *
 *  Этот класс отвечает только за коммуникацию по сети.
 *
 * ********/

class NetCommunication
{
public:
    NetCommunication() = delete;

    explicit NetCommunication(const std::string& ip_addr, int port);

    bool connect_to_server();

    bool send_message(const std::string& text);

private:
    std::string last_error;

    ba::ip::tcp::endpoint _end_point;

    ba::io_context _io_context;
    ba::ip::tcp::socket _sock;
};

using net_comm_shared = std::shared_ptr<NetCommunication>;

#endif // NETCOMMUNICATION_H
