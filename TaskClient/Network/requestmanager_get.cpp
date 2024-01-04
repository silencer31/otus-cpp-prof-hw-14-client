#include "requestmanager.h"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

bool RequestManager::send_get_username(const int id)
{
    current_request = CommandType::Get;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("get");
    json_object["type"] = QString("username");
    json_object["user_id"] = id;

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::send_get_fullname(const int id)
{
    current_request = CommandType::Get;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("get");
    json_object["type"] = QString("fullname");
    json_object["user_id"] = id;

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::send_get_userlist()
{
    current_request = CommandType::Get;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("get");
    json_object["type"] = QString("userlist");

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::send_get_tasklist(const int id)
{
    current_request = CommandType::Get;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("get");
    json_object["type"] = QString("tasklist");

    if (id >= 0) {
        json_object["user_id"] = id;
    }

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::send_get_typelist()
{
    current_request = CommandType::Get;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("get");
    json_object["type"] = QString("typelist");

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::send_get_statuslist()
{
    current_request = CommandType::Get;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("get");
    json_object["type"] = QString("statuslist");

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::send_get_taskdata(const int id)
{
    current_request = CommandType::Get;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("get");
    json_object["type"] = QString("taskdata");
    json_object["task_id"] = id;

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}
