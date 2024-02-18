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

    collector_ptr->clear_request_error(); // Очищаем предыдущую ошибку.
    collector_ptr->clear_result(); // Очищаем предыдущий результат.

    const QJsonObject reply_object = net_reply_doc.object();

    // Поле command
    if ( !reply_object.contains("command")) {
        last_error = QString("command field not found");
        return false;
    }

    // Поле error
    if ( !reply_object.contains("error")) {
        last_error = QString("error field not found");
        return false;
    }

    if (!reply_object["error"].isBool()) {
        last_error = QString("error field is not boolean");
        return false;
    }

    const bool request_error = reply_object["error"].toBool();

    const QString errtext = (reply_object.contains("errtext") ? reply_object["errtext"].toString() : QString("No error text"));

    collector_ptr->set_request_error(request_error,
                                     reply_object.contains("parameter")
                                     ? QString("%1\nParameter: %2").arg(errtext, reply_object["parameter"].toString())
                                     : errtext);

    // Поле result
    if ( !reply_object.contains("result")) {
        last_error = QString("result field not found");
        return false;
    }

    if (!reply_object["result"].isBool()) {
        last_error = QString("result field is not boolean");
        return false;
    }

    const bool request_result = reply_object["result"].toBool();

    // Поле details
    const QString details = (reply_object.contains("details") ? reply_object["details"].toString() : QString("No details"));

    // Записываем результат выполнения и детали.
    collector_ptr->set_result( request_result, details);

    // Если подтверждено наличие ошибки в запросе, выходим.
    if (request_error) { return true; }

    // Анализ ответа от сервера.
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
    case CommandType::Common:
        if (reply_object["command"] != "common") {
            last_error = QString("Command type is not common");
            return false;
        }
        return (request_result ? parse_common_request(reply_object) : true);
    case CommandType::Login:
        if (reply_object["command"] != "login") {
            last_error = QString("Command type is not login");
            return false;
        }
        return (request_result ? parse_login_request(reply_object) : true);
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
        return (request_result ? parse_get_request(reply_object) : true);
    case CommandType::Add:
        if (reply_object["command"] != "add") {
            last_error = QString("Command type is not add");
            return false;
        }
        return (request_result ? parse_add_request(reply_object) : true);
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

bool ReplyParser::parse_common_request(const QJsonObject& reply_object)
{
    if ( !reply_object.contains("type")) {
        last_error = QString("Field type not found in common request");
        return false;
    }

    if (reply_object["type"] == "usertypes") {
        return parse_get_types(reply_object);
    }

    if (reply_object["type"] == "taskstatuses") {
        return parse_get_statuses(reply_object);
    }

    last_error = QString("Unknown get request type");

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

    collector_ptr->set_id_and_type(reply_object["user_id"].toInt(0), reply_object["user_type"].toInt(0));

    return true;
}

bool ReplyParser::parse_add_request(const QJsonObject& reply_object)
{
    if ( reply_object.contains("user_id")) {
        collector_ptr->set_item_id(reply_object["user_id"].toInt(0));
        return true;
    }

    if ( reply_object.contains("task_id")) {
        collector_ptr->set_item_id(reply_object["task_id"].toInt(0));
        return true;
    }

    last_error = QString("Field with new id has NOT been found in server reply");
    return false;
}

bool ReplyParser::parse_get_request(const QJsonObject& reply_object)
{
    if ( !reply_object.contains("type")) {
        last_error = QString("Field type not found in get request");
        return false;
    }

    if (reply_object["type"] == "username") {
        return parse_get_username(reply_object);
    }

    if (reply_object["type"] == "fullname") {
        return parse_get_fullname(reply_object);
    }

    if (reply_object["type"] == "userlist") {
        return parse_get_user_ids(reply_object);
    }

    if (reply_object["type"] == "tasklist") {
        return parse_get_task_ids(reply_object);
    }

    if (reply_object["type"] == "taskdata") {
        return parse_get_taskdata(reply_object);
    }

    last_error = QString("Unknown get request type");

    return false;
}

bool ReplyParser::parse_get_username(const QJsonObject& reply_object)
{
    if (!reply_object.contains("user_type")) {
        last_error = QString("user_type field not found in get username request");
        return false;
    }

    if (!reply_object.contains("user_name")) {
        last_error = QString("user_name field not found in get username request");
        return false;
    }

    collector_ptr->set_login_and_type(reply_object["user_name"].toString(), reply_object["user_type"].toInt(0));

    return true;
}

bool ReplyParser::parse_get_fullname(const QJsonObject& reply_object)
{
    if (!reply_object.contains("second")) {
        last_error = QString("second field not found in get fullname request");
        return false;
    }

    if (!reply_object.contains("first")) {
        last_error = QString("first field not found in get fullname request");
        return false;
    }

    if (!reply_object.contains("patronymic")) {
        last_error = QString("patronymic field not found in get fullname request");
        return false;
    }

    collector_ptr->set_fullname(reply_object["second"].toString(), reply_object["first"].toString(), reply_object["patronymic"].toString());

    return true;
}

bool ReplyParser::parse_get_user_ids(const QJsonObject& reply_object)
{
    if (!reply_object.contains("id_list")) {
        last_error = QString("Field id_list not found in get userlist request");
        return false;
    }

    if ( !reply_object["id_list"].isArray()) {
        last_error = QString("Field id_list in get userlist request is not array");
        return false;
    }

    const QJsonArray jarray = reply_object["id_list"].toArray();

    collector_ptr->prepare_user_ids(jarray.size());

    foreach (QJsonValue jvalue, jarray) {
        collector_ptr->user_ids_push_back(jvalue.toInt(0));
    }

    return true;
}

bool ReplyParser::parse_get_task_ids(const QJsonObject& reply_object)
{
    if (!reply_object.contains("id_list")) {
        last_error = QString("Field id_list not found in get tasklist request");
        return false;
    }

    if ( !reply_object["id_list"].isArray()) {
        last_error = QString("Field id_list in get tasklist request is not array");
        return false;
    }

    const QJsonArray jarray = reply_object["id_list"].toArray();

    collector_ptr->prepare_task_ids(jarray.size());

    foreach (QJsonValue jvalue, jarray) {
        collector_ptr->task_ids_push_back(jvalue.toInt(0));
    }

    return true;
}

bool ReplyParser::parse_get_types(const QJsonObject& reply_object)
{
    if (!reply_object.contains("numbers")) {
        last_error = QString("Field numbers not found in get typelist request");
        return false;
    }

    if (!reply_object.contains("descriptions")) {
        last_error = QString("Field descriptions not found in get typelist request");
        return false;
    }

    if ( !reply_object["numbers"].isArray()) {
        last_error = QString("Field numbers in get typelist request is not array");
        return false;
    }

    if ( !reply_object["descriptions"].isArray()) {
        last_error = QString("Field id_list in get typelist request is not array");
        return false;
    }

    const QJsonArray numbers = reply_object["numbers"].toArray();
    const QJsonArray descriptions = reply_object["descriptions"].toArray();

    if (numbers.size() != descriptions.size()) {
        last_error = QString("Fields numbers and descriptions have different sizes in get typelist request");
        return false;
    }

    collector_ptr->prepare_user_types(numbers.size());

    for(int i = 0; i < numbers.size(); ++i) {
        collector_ptr->user_types_push_back(numbers.at(i).toInt(0), descriptions.at(i).toString());
    }

    return true;
}

bool ReplyParser::parse_get_statuses(const QJsonObject& reply_object)
{
    if (!reply_object.contains("numbers")) {
        last_error = QString("Field numbers not found in get statuslist request");
        return false;
    }

    if (!reply_object.contains("descriptions")) {
        last_error = QString("Field descriptions not found in get statuslist request");
        return false;
    }

    if ( !reply_object["numbers"].isArray()) {
        last_error = QString("Field numbers in get statuslist request is not array");
        return false;
    }

    if ( !reply_object["descriptions"].isArray()) {
        last_error = QString("Field id_list in get statuslist request is not array");
        return false;
    }

    const QJsonArray numbers = reply_object["numbers"].toArray();
    const QJsonArray descriptions = reply_object["descriptions"].toArray();

    if (numbers.size() != descriptions.size()) {
        last_error = QString("Fields numbers and descriptions have different sizes in get statuslist request");
        return false;
    }

    collector_ptr->prepare_task_statuses(numbers.size());

    for(int i = 0; i < numbers.size(); ++i) {
        collector_ptr->task_statuses_push_back(numbers.at(i).toInt(0), descriptions.at(i).toString());
    }

    return true;
}

bool ReplyParser::parse_get_taskdata(const QJsonObject& reply_object)
{
    if (!reply_object.contains("user_id")) {
        last_error = QString("Field user_id not found in get taskdata request");
        return false;
    }

    if (!reply_object.contains("status")) {
        last_error = QString("Field status not found in get taskdata request");
        return false;
    }

    if (!reply_object.contains("deadline")) {
        last_error = QString("Field deadline not found in get taskdata request");
        return false;
    }

    if (!reply_object.contains("name")) {
        last_error = QString("Field name not found in get taskdata request");
        return false;
    }

    if (!reply_object.contains("description")) {
        last_error = QString("Field description not found in get taskdata request");
        return false;
    }

    collector_ptr->set_task_data(reply_object["user_id"].toInt(0), reply_object["status"].toInt(0),
            reply_object["deadline"].toString(), reply_object["name"].toString(), reply_object["description"].toString());

    return true;
}
