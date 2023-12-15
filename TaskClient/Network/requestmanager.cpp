#include "requestmanager.h"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>

bool RequestManager::get_server_answer(std::string& answer)
{
    if ( !net_communication_ptr->read_answer()) {
        return false;
    }

    answer = net_communication_ptr->get_answer();

    return !answer.empty();
}

bool RequestManager::send_test_request()
{
    current_request = CommandType::Test;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("test");

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::send_login(const QString& user_name, const QString& password)
{
    current_request = CommandType::Login;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("login");
    json_object["username"] = user_name;
    json_object["password"] = password;

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::send_closedown()
{
    current_request = CommandType::Closedown;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("closedown");

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::send_shutdown()
{
    current_request = CommandType::Shutdown;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("shutdown");

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}
