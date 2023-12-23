#include "collector.h"

// Установить наличие ошибки в запросе и описание.
void Collector::set_request_error(bool value, const QString& text)
{
    request_error.error = value;
    request_error.error_text = text;
}

// Очистить предыдущую ошибку зароса.
void Collector::clear_request_error()
{
    request_error.error = false;
    request_error.error_text.clear();
}

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
void Collector::prepare_user_ids(const int size)
{
    user_ids.id_list.clear();
    user_ids.id_list.reserve(size);
    user_ids.size = size;
}

void Collector::prepare_task_ids(const int size)
{
    task_ids.id_list.clear();
    task_ids.id_list.reserve(size);
    task_ids.size = size;
}

// Подготовить коллекцию с типами пользователей.
void Collector::prepare_user_types(const int size)
{
    user_types.descriptions.clear();
    user_types.size = size;
}

// Подготовить коллекцию со статусами задач.
void Collector::prepare_task_statuses(const int size)
{
    task_statuses.descriptions.clear();
    task_statuses.size = size;
}
