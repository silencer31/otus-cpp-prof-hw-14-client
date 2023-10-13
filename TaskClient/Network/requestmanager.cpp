#include "requestmanager.h"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>

bool RequestManager::send_test()
{
    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("test");

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::login_on_server(const std::string& login, const std::string& password)
{
    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("login");
    json_object["username"] = QString::fromStdString(login);
    json_object["password"] = QString::fromStdString(password);

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::shutdown_server(const int user_id, const std::string& password)
{
    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("login");
    json_object["user_unique_is"] = user_id;
    json_object["password"] = QString::fromStdString(password);

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}
