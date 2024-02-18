#ifndef NETCOMMUNICATION_H
#define NETCOMMUNICATION_H

#pragma once

#include <boost/asio.hpp>
#include <string>

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

    // Подключиться снова.
    bool reconnect();

    // Отключиться
    void disconnect();

    // Узнать, удалось ли подключиться.
    bool is_connected() const {
        return connected;
    }

    // Отправка сообщения/запроса на сервер.
    bool send_message(const char* data_ptr, const std::size_t data_size);

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

using net_comm_unique = std::unique_ptr<NetCommunication>;

#endif // NETCOMMUNICATION_H
