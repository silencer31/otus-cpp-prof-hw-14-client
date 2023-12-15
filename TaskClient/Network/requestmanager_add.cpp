#include "requestmanager.h"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

bool RequestManager::send_add_user(const QString& user_name, int user_type, const QString& pass,
                   const QString& second, const QString& first, const QString& patron)
{
    current_request = CommandType::Add;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("add");
    json_object["type"] = QString("user");
    json_object["username"] = user_name;
    json_object["user_type"] = user_type;
    json_object["password"] = pass;
    json_object["second"] = second;
    json_object["first"] = first;
    json_object["patronymic"] = patron;

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::send_add_task(const int user_id, const QString& deadline, const QString& name, const QString& description)
{
    current_request = CommandType::Add;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("add");
    json_object["type"] = QString("task");
    json_object["user_id"] = user_id;
    json_object["deadline"] = deadline;
    json_object["name"] = name;
    json_object["description"] = description;

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}
