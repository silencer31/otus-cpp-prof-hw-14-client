#ifndef DATA_COLLECTIONS_H
#define DATA_COLLECTIONS_H

#pragma once

#include <QString>
#include <QVector>

//
// test, closedown, shutdown
// add : user, task
// del : user, task
// set : password, usertype, taskstatus, taskuser
struct ReplyResult
{
    bool result;
    QString details;
};

// Для ответа на запрос логин клиента.
// login
struct Login
{
    int user_id;
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
struct IntStrArrays
{
    int size;
    QVector<int> numbers;
    QVector<QString>  str_vect;
};

#endif // DATA_COLLECTIONS_H
