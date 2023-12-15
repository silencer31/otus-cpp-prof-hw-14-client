#include "collector.h"

void Collector::set_result(bool result, const QString& details)
{
    reply_result.result = result;
    reply_result.details = details;
}

// Очистить предыдущий результат.
void Collector::clear_result()
{
    reply_result.result = false;
    reply_result.details.clear();
}

void Collector::set_login(int id, int type)
{
    login.user_id = id;
    login.user_type = type;
}

void Collector::set_fullname(const QString& second, const QString& first, const QString& patron)
{
    fullname.second = second;
    fullname.first = first;
    fullname.patronymic = patron;
}

void Collector::set_task_data(int id, int status, const QString& dl, const QString& name, const QString& desc)
{
    task_data.user_id = id;
    task_data.status = status;
    task_data.deadline = dl;
    task_data.name = name;
    task_data.description = desc;
}

// Подготовить вектор с числами для заполнения.
void Collector::prepare_int_array(const int size)
{
    int_array.id_list.clear();
    int_array.id_list.reserve(size);
    int_array.size = size;
}

// Добавить число в вектор с числами.
void Collector::int_array_push_back(const int value)
{
    int_array.id_list.push_back(value);
}

// Получить число по индексу.
int Collector::get_ia_value_by_index(const int index)
{
    return int_array.id_list.at(index);
}

// Константный итератор на начало массива с числами.
QVector<int>::const_iterator Collector::int_array_cib()
{
    return int_array.id_list.constBegin();
}

// Константный итератор на конец массива с числами.
QVector<int>::const_iterator Collector::int_array_cie()
{
    return int_array.id_list.constEnd();
}

// Подготовить вектор с числами и строками для заполнения.
void Collector::prepare_int_str_map(const int size)
{
    int_str_map.descriptions.clear();
    int_str_map.size = size;
}

// Добавить число и строку.
void Collector::int_str_map_push_back(const int value, const QString& str_value)
{
    int_str_map.descriptions[value] = str_value;
}

// Константный итератор на начало коллекции.
QMap<int, QString>::const_iterator Collector::int_str_map_str_cib()
{
    return int_str_map.descriptions.constBegin();
}

// Константный итератор на конец коллекции.
QMap<int, QString>::const_iterator Collector::int_str_map_str_cie()
{
    return int_str_map.descriptions.constEnd();
}
