#include "netcommunication.h"

NetCommunication::NetCommunication(const std::string& ip_addr, int port)
    : _end_point(ba::ip::tcp::endpoint(ba::ip::address::from_string(ip_addr), port))
    , _sock(ba::ip::tcp::socket(_io_context))
{
    connected = connect_to_server();
}

NetCommunication::~NetCommunication()
{
    disconnect();
}

// Подключаемся к серверу.
bool NetCommunication::connect_to_server()
{
    try {
        _sock.connect(_end_point);
    }
    catch(const boost::system::system_error& ex) {
        last_error = std::string("boost exception: ") + ex.what();
        return false;
    }
    catch(const std::exception& ex) {
        last_error = std::string("std exception: ") + ex.what();
        return false;
    }

    return true;
}

// Отключиться
void NetCommunication::disconnect()
{
    if ( !_sock.is_open() ) {
        return;
    }

    boost::system::error_code ignore;

    _sock.shutdown(
        boost::asio::ip::tcp::socket::shutdown_both,
        ignore
        );

    _sock.close(ignore);
}

// Отправка сообщения/запроса на сервер.
bool NetCommunication::send_message(const char* data_ptr, const std::size_t data_size)
{
    if (data_size == 0) {
        last_error = std::string("Empty data to send");
        return false;
    }

    request_size = data_size;
    bytes_written = 0;

    try {
        bytes_written = ba::write(_sock, ba::buffer(data_ptr, data_size));
    }
    catch(const boost::system::system_error& ex) {
        last_error = std::string("boost exception: ") + ex.what();
        return false;
    }
    catch(const std::exception& ex) {
        last_error = std::string("std exception: ") + ex.what();
        return false;
    }

    // Проверяем, сколько байт удалось записать.
    if (bytes_written == 0) {
        last_error = "Zero bytes were written";
        return false;
    }

    return true;
}

// Чтение ответа от сервера.
bool NetCommunication::read_answer()
{
    char data[1024]; // Буфер для ответа от сервера.

    for( int i = 0; i<1024; ++i) {
        data[i] = 0;
    }

    try {
        answer_len = _sock.read_some( ba::buffer(data) );
    }
    catch(const boost::system::system_error& ex) {
        last_error = std::string("boost exception: ") + ex.what();
        return false;
    }
    catch(const std::exception& ex) {
        last_error = std::string("std exception: ") + ex.what();
        return false;
    }

    // Проверяем, сколько байт удалось прочитать.
    if (answer_len == 0) {
        last_error = "Zero bytes were read";
        return false;
    }

    answer.clear();
    answer.reserve(answer_len);
    answer.assign(data);

    return true;
}
