#include "netcommunication.h"

NetCommunication::NetCommunication(const std::string& ip_addr, int port) :
    _end_point(ba::ip::tcp::endpoint(ba::ip::address::from_string(ip_addr), port)),
    _sock(ba::ip::tcp::socket(_io_context))
{
    connected = connect_to_server();
}

NetCommunication::~NetCommunication()
{
    if ( _sock.is_open() ) {

        boost::system::error_code ignore;

        _sock.shutdown(
            boost::asio::ip::tcp::socket::shutdown_both,
            ignore
            );

        _sock.close(ignore);
    }
}

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

bool NetCommunication::send_message(const std::string& text)
{
    try {
        ba::write(_sock, ba::buffer(text.data(), text.size()));

        char data[1024]; // Буфер для ответа от сервера.

        for( int i = 0; i<1024; ++i) {
            data[i] = 0;
        }

        size_t len = _sock.read_some( ba::buffer(data) );
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
