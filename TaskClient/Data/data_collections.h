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
    LoginType() = default;

    explicit LoginType(const QString& un, const int& ut)
        : user_name(un), user_type(ut) {}

    QString user_name;
    int user_type;
};

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
