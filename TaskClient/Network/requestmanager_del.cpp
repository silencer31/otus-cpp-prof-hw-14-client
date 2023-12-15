#include "requestmanager.h"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

bool RequestManager::send_del_user(const int id)
{
    current_request = CommandType::Del;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("del");
    json_object["type"] = QString("user");
    json_object["user_id"] = id;

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}

bool RequestManager::send_del_task(const int id)
{
    current_request = CommandType::Del;

    QJsonDocument json_document;
    QJsonObject   json_object;

    json_object["command"] = QString("del");
    json_object["type"] = QString("task");
    json_object["task_id"] = id;

    json_document.setObject(json_object);

    return net_communication_ptr->send_message(json_document.toJson().constData(), json_document.toJson().length());
}
