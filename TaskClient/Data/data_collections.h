#ifndef DATA_COLLECTIONS_H
#define DATA_COLLECTIONS_H

#pragma once

#include <QString>
#include <QVector>
#include <QMap>

struct RequestError
{
    bool error; // Наличие ошибки/проблемы в самом запросе.
    QString error_text; // Текст ошибки.
};

//
// test, closedown, shutdown
// add : user, task
// del : user, task
// set : password, usertype, taskstatus, taskuser
// get : username, fullname, taskdata, userlist, tasklist, statuslist, typelist
struct ReplyResult
{
    bool result; // Результат выполнения запроса.
    QString details;
};

// Для ответа на запрос логин клиента.
// login
struct IdType
{
    int user_id;
    int user_type;
};

// get : username
struct LoginType
{
    QString user_name;
    int user_type;
};

// get : fullname
struct Fullname
{
    QString second; // Фамилия.
    QString first;  // Имя.
    QString patronymic; // Отчество.
};

// get : taskdata
struct TaskData
{
    int user_id;
    int status;
    QString deadline;
    QString name;
    QString description;
};

// get : userlist, tasklist
struct IntArray
{
    int size;
    QVector<int> id_list;
};

// get : statuslist, typelist
struct IntStrMap
{
    int size;
    QMap<int, QString> descriptions;
};

#endif // DATA_COLLECTIONS_H
