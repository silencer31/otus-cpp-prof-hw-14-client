#include "netcommunication.h"

NetCommunication::NetCommunication(const std::string& ip_addr, int port) :
    _end_point(ba::ip::tcp::endpoint(ba::ip::address::from_string(ip_addr), port)),
    _sock(ba::ip::tcp::socket(_io_context))
{

}

bool NetCommunication::connect_to_server()
{
    bool result = false;

    try {
        _sock.connect(_end_point);
        result = true;
    }
    catch(const boost::system::system_error& ex) {
        last_error = std::string("boost exception: ") + ex.what();
    }
    catch(const std::exception& ex) {
        last_error = std::string("std exception: ") + ex.what();
    }

    return result;
}

bool NetCommunication::send_message(const std::string& text)
{
    bool result = false;

    try {
        ba::write(_sock, ba::buffer(text.data(), text.size()));

        char data[1024];

        for( int i = 0; i<1024; ++i) {
            data[i] = 0;
        }

        size_t len = _sock.read_some( ba::buffer(data) );

        result = true;
    }
    catch(const boost::system::system_error& ex) {
        last_error = std::string("boost exception: ") + ex.what();
    }
    catch(const std::exception& ex) {
        last_error = std::string("std exception: ") + ex.what();
    }

    return result;
}
