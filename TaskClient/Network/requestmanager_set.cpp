#include "requestmanager.h"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

bool RequestManager::send_set_password(const int user_id, const QString& password)
{
    current_request = CommandType::Set;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("set");
    json_object["type"] = QString("password");
    json_object["user_id"] = user_id;
    json_object["password"] = password;

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::send_set_usertype(const int user_id, const int user_type)
{
    current_request = CommandType::Set;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("set");
    json_object["type"] = QString("usertype");
    json_object["user_id"] = user_id;
    json_object["user_type"] = user_type;

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::send_set_taskstatus(const int task_id, const int status)
{
    current_request = CommandType::Set;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("set");
    json_object["type"] = QString("taskstatus");
    json_object["task_id"] = task_id;
    json_object["status"] = status;

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::send_set_taskuser(const int task_id, const int user_id)
{
    current_request = CommandType::Set;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("set");
    json_object["type"] = QString("taskuser");
    json_object["task_id"] = task_id;
    json_object["user_id"] = user_id;

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}


