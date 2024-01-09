#ifndef COLLECTOR_H
#define COLLECTOR_H

#pragma once

#include <memory>
#include "data_collections.h"

/***********
 *
 *  Этот класс сохраняет последние ответы от сервера в виде коллекций данных.
 *
 * ********/

class Collector
{
public:
    Collector() = default;
    ~Collector() = default;

    /// Была ли ошибка в запросе клиента к серверу.
    // Узнать была ли ошибка.
    bool req_error() {
        return request_error.error;
    }

    // Получить описание ошибки в запросе.
    const QString& get_error_text() {
        return request_error.error_text;
    }

    // Записать наличие ошибки в запросе и её описание.
    void set_request_error(bool value, const QString& text);

    // Очистить предыдущую ошибку зароса.
    void clear_request_error();

    /// Результат выполнения запроса (Получилось или нет).
    // Получить результат
    bool get_result() {
        return reply_result.result;
    }

    // Получить детали/описание причины.
    const QString& get_details() {
        return reply_result.details;
    }

    // Записать результат и детали выполнения.
    void set_result(bool result, const QString& details);

    // Очистить предыдущий результат.
    void clear_result();

    /// Логин пользователя на сервере.
    // Получить id пользователя в базе.
    int get_own_id() {
        return id_type.user_id;
    }

    // Получить тип пользователя в базе.
    int get_own_type() {
        return id_type.user_type;
    }

    // Записать id и тип пользователя.
    void set_id_and_type(int id, int type);

    /// Логин и тип другого пользователя в базе(не свои).
    // Получить чужую пару логин и тип.
    const LoginType& get_login_and_type() {
        return login_type;
    }

    // Установить username и тип пользователя.
    void set_login_and_type(const QString& username, int type);

    /// ФИО пользователя.
    // Получить структуру с ФИО
    const Fullname& get_fullname() {
        return fullname;
    }

    // Установить ФИО.
    void set_fullname(const QString& second, const QString& first, const QString& patron);


    /// Данные одной задачи.
    // Получить структуру с данными задачи.
    const TaskData& get_task_data() {
        return task_data;
    }

    int get_task_user_id() {
        return task_data.user_id;
    }

    int get_task_status() {
        return task_data.status;
    }

    const QString& get_task_name() {
        return task_data.name;
    }

    const QString& get_task_deadline() {
        return task_data.deadline;
    }

    const QString& get_task_description() {
        return task_data.description;
    }

    // Установить данные задачи.
    void set_task_data(int id, int status, const QString& dl, const QString& name, const QString& desc);


    /// Коллекция - набор id пользователей.
    // Есть ли данные в коллекции.
    bool user_ids_received() {
        return !user_ids.id_list.isEmpty();
    }

    // Подготовить вектор с числами для заполнения.
    void prepare_user_ids(const int size);

    // Добавить id в вектор с id пользователей.
    void user_ids_push_back(const int value) {
        user_ids.id_list.push_back(value);
    }

    // Получить id пользователя по индексу.
    int get_user_id_by_index(const int index) {
        return user_ids.id_list.at(index);
    }

    // Константный итератор на начало массива с id пользователей.
    QVector<int>::const_iterator user_ids_cib() {
        return user_ids.id_list.constBegin();
    }

    // Константный итератор на конец массива с id пользователей.
    QVector<int>::const_iterator user_ids_cie() {
        return user_ids.id_list.constEnd();
    }


    /// Коллекция - набор id задач.
    // Есть ли данные в коллекции.
    bool task_ids_received() {
        return !task_ids.id_list.isEmpty();
    }

    // Подготовить вектор с id задач для заполнения.
    void prepare_task_ids(const int size);

    // Добавить число в вектор с id задач.
    void task_ids_push_back(const int value) {
        task_ids.id_list.push_back(value);
    }

    // Получить id задачи по индексу.
    int get_task_id_by_index(const int index) {
        return task_ids.id_list.at(index);
    }

    // Константный итератор на начало массива с id задач.
    QVector<int>::const_iterator task_ids_cib() {
        return task_ids.id_list.constBegin();
    }

    // Константный итератор на конец массива с id задач.
    QVector<int>::const_iterator task_ids_cie() {
        return task_ids.id_list.constEnd();
    }


    /// Коллекция - типы пользователей.
    // Есть ли данные в коллекции.
    bool user_types_received() {
        return !user_types.descriptions.isEmpty();
    }

    // Подготовить коллекцию с типами пользователей.
    void prepare_user_types(const int size);

    // Добавить номер типа и описание.
    void user_types_push_back(const int value, const QString& str_value) {
        user_types.descriptions[value] = str_value;
    }

    // Текстовое соответствие номеру типа пользователя.
    QString type_description(const int value) {
        return user_types.descriptions.contains(value) ? user_types.descriptions[value] : QString();
    }

    // Константный итератор на начало коллекции с типами пользователей.
    QMap<int, QString>::const_iterator user_types_cib() {
        return user_types.descriptions.constBegin();
    }

    // Константный итератор на конец коллекции с типами пользователей.
    QMap<int, QString>::const_iterator user_types_cie() {
        return user_types.descriptions.constEnd();
    }


    /// Коллекция - статусы задач.
    // Есть ли данные в коллекции.
    bool task_statuses_received() {
        return !task_statuses.descriptions.isEmpty();
    }

    // Подготовить коллекцию со статусами задач.
    void prepare_task_statuses(const int size);

    // Добавить номер статуса и описание.
    void task_statuses_push_back(const int value, const QString& str_value) {
        task_statuses.descriptions[value] = str_value;
    }

    // Текстовое соответствие номеру статуса задачи.
    QString status_description(const int value) {
        return task_statuses.descriptions.contains(value) ? task_statuses.descriptions[value] : QString();
    }

    // Константный итератор на начало коллекции со статусами задач.
    QMap<int, QString>::const_iterator task_statuses_cib() {
        return task_statuses.descriptions.constBegin();
    }

    // Константный итератор на конец коллекции со статусами задач.
    QMap<int, QString>::const_iterator task_statuses_cie() {
        return task_statuses.descriptions.constEnd();
    }

    /// id, полученный от сервера после последнего запроса на создание пользователя или задачи.
    void set_item_id(const int value) {
        item_id = value;
    }

    // Узнать id, полученный от сервера после последнего запроса.
    int get_item_id() {
        return item_id;
    }

private:
    RequestError request_error;
    ReplyResult  reply_result;
    IdType       id_type;
    LoginType    login_type;
    Fullname     fullname;
    TaskData     task_data;
    IntArray     user_ids;      // get : userlist
    IntArray     task_ids;      // get : tasklist
    IntStrMap    user_types;    // get : typelist
    IntStrMap    task_statuses; // get : statuslist

    int item_id; // id, полученный от сервера на запрос создания нового пользователя или задачи.
};

using collector_shared = std::shared_ptr<Collector>;

#endif // COLLECTOR_H
