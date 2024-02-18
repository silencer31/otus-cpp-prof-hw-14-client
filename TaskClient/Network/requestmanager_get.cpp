#include "requestmanager.h"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

// Получить список возможных типов пользователя и список с их описанием.
bool RequestManager::send_get_user_types()
{
    current_request = CommandType::Common;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("common");
    json_object["type"] = QString("usertypes");

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

// Получить список возможных статусов задачи и список с их описанием.
bool RequestManager::send_get_task_statuses()
{
    current_request = CommandType::Common;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("common");
    json_object["type"] = QString("taskstatuses");

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

// Получить логин и тип пользователя по user id.
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

// Получить ФИО пользователя по user id.
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

// Получить список всех id пользователей.
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

// Если указан user_id, получить список его задач, а если не указан, то список всех задач.
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

// Получить данные задачи по task id.
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
