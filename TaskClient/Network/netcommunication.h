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

    ~NetCommunication();

    bool connect_to_server();

    bool send_message(const std::string& text);

    bool is_connected() {
        return connected;
    }

    const std::string& get_last_error() const {
        return last_error;
    }

    const std::string& get_answer() const {
        return answer;
    }

private:
    ba::io_context _io_context;
    ba::ip::tcp::endpoint _end_point;
    ba::ip::tcp::socket _sock;

    bool connected;

    std::string last_error;

    std::string answer; // Ответ сервера.
    size_t answer_len;  // Длина ответа.
};

using net_comm_shared = std::shared_ptr<NetCommunication>;

#endif // NETCOMMUNICATION_H
