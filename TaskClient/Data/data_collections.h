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

// Для ответа от сервера на запрос сделать логин в новой сессии не сервере.
// login
struct IdType
{
    int user_id;
    int user_type;
};

// Чтобы получить от сервера логин и тип пользователя по user_id.
// get : username
struct LoginType
{
    LoginType() = default;

    explicit LoginType(const QString& un, const int& ut)
        : user_name(un), user_type(ut) {}

    QString user_name;
    int user_type;
};

// Чтобы получить от сервера ФИО пользователя по user_id.
// get : fullname
struct Fullname
{
    Fullname() = default;

    explicit Fullname(const QString& sval, const QString& fval, const QString& pval)
        : second(sval), first(fval), patronymic(pval) {}

    QString second; // Фамилия.
    QString first;  // Имя.
    QString patronymic; // Отчество.
};

// Данная структура объединяет в себе логин, тип и ФИО пользователя.
struct UserData
{
    UserData() = default;

    /*explicit UserData(const QString& un, const int& ut, const QString& sval, const QString& fval, const QString& pval)
        : login_type.user_name(un), login_type.user_type(ut) {}*/

    explicit UserData(const LoginType& lt, const Fullname& fn)
        : login_type(lt), fullname(fn) {}

    LoginType login_type;
    Fullname  fullname;
};

// Струтура содержит в себе id исполнителя  задачи, статус задачи, срок исполнения, название и описание задачи.
// get : taskdata
struct TaskData
{
    TaskData() = default;

    explicit TaskData(const int& uid, const int& st, const QString& dlval, const QString& nval, const QString& descval)
        : user_id(uid), status(st), deadline(dlval), name(nval), description(descval) {}

    int user_id;
    int status;
    QString deadline;
    QString name;
    QString description;
};

// Массив предназначен для получения от сервера списка id пользователей или id задач.
// get : userlist, tasklist
struct IntArray
{
    int size;
    QVector<int> id_list;
};

// Для получения возможных типов пользователя и статусов задач.
// get : statuslist, typelist
struct IntStrMap
{
    int size;
    QMap<int, QString> descriptions;
};

#endif // DATA_COLLECTIONS_H
