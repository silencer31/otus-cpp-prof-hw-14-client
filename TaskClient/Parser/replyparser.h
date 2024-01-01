#ifndef REPLYPARSER_H
#define REPLYPARSER_H

#pragma once

#include <memory>
#include <string>

#include <QString>
#include <QtCore/QJsonObject>

#include "Data/collector.h"
#include "Data/data_types.h"

/***********
 *
 *  Этот класс парсит ответы от сервера в формате json в коллекции данных.
 *
 * ********/

class ReplyParser
{
public:
    ReplyParser() = delete;

    explicit ReplyParser(const collector_shared ctor_ptr)
        : collector_ptr(ctor_ptr) {};

    ~ReplyParser() = default;

    bool handle_reply(CommandType command, const std::string& reply);

    bool parse_login_request(const QJsonObject& reply_object);

    bool parse_get_request(const QJsonObject& reply_object);

    bool parse_get_username(const QJsonObject& reply_object);

    bool parse_get_fullname(const QJsonObject& reply_object);

    bool parse_get_user_ids(const QJsonObject& reply_object);

    bool parse_get_task_ids(const QJsonObject& reply_object);

    bool parse_get_types(const QJsonObject& reply_object);

    bool parse_get_statuses(const QJsonObject& reply_object);


    bool parse_get_taskdata(const QJsonObject& reply_object);

    const QString& get_last_error() const {
        return last_error;
    }

private:
    const collector_shared collector_ptr; // Сборщик данных - ответов от сервера.

    QString last_error;
};

using parser_shared = std::shared_ptr<ReplyParser>;

#endif // REPLYPARSER_H
