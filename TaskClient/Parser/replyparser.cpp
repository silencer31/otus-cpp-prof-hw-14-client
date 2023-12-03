#include "replyparser.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>

bool ReplyParser::handle_reply(CommandType command, const std::string& reply)
{
    QJsonParseError parse_result;

    QJsonDocument net_reply_doc = QJsonDocument::fromJson( QByteArray::fromStdString(reply), &parse_result);

    if (parse_result.error != QJsonParseError::NoError) {
        last_error = parse_result.errorString();
        return false;
    }

    if ( net_reply_doc.isNull() ) { // Данные получены в некорректном формате.
        last_error = QString("json doc is null");
        return false;
    }

    // Пустой ответ от сервера расцениваем как ошибку.
    if ( net_reply_doc.isEmpty() ) {
        last_error = QString("json doc is empty");
        return false;
    }

    if ( !net_reply_doc.isObject()) {
        last_error = QString("json doc is not json object");
        return false;
    }

    const QJsonObject reply_object = net_reply_doc.object();

    if ( !reply_object.contains("command")) {
        last_error = QString("command field not found");
        return false;
    }

    if ( !reply_object.contains("result")) {
        last_error = QString("result field not found");
        return false;
    }

    if (!reply_object["result"].isBool()) {
        last_error = QString("result field is not boolean");
        return false;
    }

    const QString details = (reply_object.contains("details") ? reply_object["details"].toString() : QString("No deatils"));

    collector_ptr->set_result(reply_object["result"].toBool(), details);

    switch(command) {
    case CommandType::Unknown:
        last_error = QString("Bad command type");
        return false;
    case CommandType::Test:
        if (reply_object["command"] != "test") {
            last_error = QString("Command type is not test");
            return false;
        }
        return true;
    case CommandType::Login:
        if (reply_object["command"] != "login") {
            last_error = QString("Command type is not login");
            return false;
        }
        return parse_login_request(reply_object);
    case CommandType::Closedown:
        if (reply_object["command"] != "closedown") {
            last_error = QString("Command type is not closedown");
            return false;
        }
        return true;
    case CommandType::Shutdown:
        if (reply_object["command"] != "shutdown") {
            last_error = QString("Command type is not shutdown");
            return false;
        }
        return true;
    case CommandType::Get:
        if (reply_object["command"] != "get") {
            last_error = QString("Command type is not get");
            return false;
        }
        return parse_get_request(reply_object);
    case CommandType::Add:
        if (reply_object["command"] != "add") {
            last_error = QString("Command type is not add");
            return false;
        }
        return true;
    case CommandType::Set:
        if (reply_object["command"] != "set") {
            last_error = QString("Command type is not set");
            return false;
        }
        return true;
    case CommandType::Del:
        if (reply_object["command"] != "del") {
            last_error = QString("Command type is not del");
            return false;
        }
        return true;
    default:
        last_error = QString("Bad command type");
        return false;
    }

    return false;
}

bool ReplyParser::parse_login_request(const QJsonObject& reply_object)
{
    if (!reply_object.contains("user_id")) {
        last_error = QString("user_id field not found");
        return false;
    }

    if (!reply_object.contains("user_type")) {
        last_error = QString("user_type field not found");
        return false;
    }

    collector_ptr->set_login(reply_object["user_id"].toInt(0), reply_object["user_type"].toInt(0));

    return true;
}

bool ReplyParser::parse_get_request(const QJsonObject& reply_object)
{
    return true;
}
