#ifndef COLLECTOR_H
#define COLLECTOR_H

#pragma once

#include <memory>
#include "data_collections.h"

/***********
 *
 *  Этот класс сохраняет последние ответы от сервера в виде коллекцй данных.
 *
 * ********/

class Collector
{
public:
    Collector() = default;
    ~Collector() = default;

    /// Результат выполнения запроса (Получилось или нет).
    // Получить результат
    bool get_result() {
        return reply_result.result;
    }

    // Получить детали/описание причины.
    const QString& get_details() {
        return reply_result.details;
    }

    // Установить результат и описание.
    void set_result(bool result, const QString& details);

    // Очистить предыдущий результат.
    void clear_result();

    /// Логин пользователя на сервере.
    // Получить id пользователя в базе.
    int get_user_id() {
        return login.user_id;
    }

    // Получить тип пользователя в базе.
    int get_user_type() {
        return login.user_type;
    }

    // Установить id и тип пользователя.
    void set_login(int id, int type);


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

    // Установить данные задачи.
    void set_task_data(int id, int status, const QString& dl, const QString& name, const QString& desc);


    /// Коллекция - вектор с числами.
    // Подготовить вектор с числами для заполнения.
    void prepare_int_array(const int size);

    // Добавить число в вектор с числами.
    void int_array_push_back(const int value);

    // Получить число по индексу.
    int get_ia_value_by_index(const int index);

    // Константный итератор на начало массива с числами.
    QVector<int>::const_iterator int_array_cib();

    // Константный итератор на конец массива с числами.
    QVector<int>::const_iterator int_array_cie();


    /// Коллекция с вектором с числами и вектором строк.
    // Подготовить вектор с числами и строками для заполнения.
    void prepare_int_str_array(const int size);

    // Добавить число и строку.
    void int_str_array_push_back(const int value, const QString& str_value);

    // Получить число по индексу.
    int get_isa_number_by_index(const int index);

    // Получить число по индексу.
    QString get_isa_string_by_index(const int index);

    // Константный итератор на начало массива с числами.
    QVector<int>::const_iterator int_str_array_int_cib();

    // Константный итератор на конец массива с числами.
    QVector<int>::const_iterator int_str_array_int_cie();

    // Константный итератор на начало массива со строками.
    QVector<QString>::const_iterator int_str_array_str_cib();

    // Константный итератор на конец массива со строками.
    QVector<QString>::const_iterator int_str_array_str_cie();

private:
    ReplyResult  reply_result;
    Login        login;
    Fullname     fullname;
    TaskData     task_data;
    IntArray     int_array;
    IntStrArrays int_str_arrays;
};

using collector_shared = std::shared_ptr<Collector>;

#endif // COLLECTOR_H
