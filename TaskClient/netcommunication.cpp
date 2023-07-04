#include "netcommunication.h"

NetCommunication::NetCommunication()
{

}

bool NetCommunication::connect_to_server()
{
    bool result = false;

    try {
        _sock.connect(_ep);
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
