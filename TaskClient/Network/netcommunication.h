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

    // Подключаемся к серверу.
    bool connect_to_server();

    // Узнать, удалось ли подключиться.
    bool is_connected() const {
        return connected;
    }

    // Отправка сообщения.
    bool send_message(const std::string& text);

    // Чтение ответа от сервера.
    bool read_answer();

    // Ошибка, полученная в процессе сетевого взаимодействия.
    const std::string& get_last_error() const {
        return last_error;
    }

    // Получить ответ от сервера.
    const std::string& get_answer() const {
        return answer;
    }

private:
    ba::io_context _io_context;
    ba::ip::tcp::endpoint _end_point;
    ba::ip::tcp::socket _sock;

    bool connected;

    std::string last_error; // Ошибка, полученная в процессе сетевого взаимодействия.

    std::size_t request_size;  // Размер данных в сетевом запросе.
    std::size_t bytes_written; // Сколько байт удалось отправить.

    std::string answer; // Ответ сервера.
    std::size_t answer_len;  // Длина ответа.
};

using net_comm_shared = std::shared_ptr<NetCommunication>;

#endif // NETCOMMUNICATION_H
