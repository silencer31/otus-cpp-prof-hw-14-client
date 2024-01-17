#include "data_keeper.h"

// Узнать, есть ли уже такой логин среди полученных.
bool DataKeeper::user_name_received(const QString& login)
{
    for(auto iter = users_data.constBegin(); iter != users_data.constEnd(); ++iter) {
        if (login == iter->login_type.user_name) {
            return true;
        }
    }

    return false;
}

// Узнать, есть ли уже такая задача среди полученных.
bool DataKeeper::task_name_received(const QString& task_name)
{
    for(auto iter = tasks_data.constBegin(); iter != tasks_data.constEnd(); ++iter) {
        if (task_name == iter->name) {
            return true;
        }
    }

    return false;
}

// Для всех задач, на которые назначен переданный user_id установить user_id = 0 и сбросить статус на Not appointed.
void DataKeeper::reset_user_tasks(const int user_id)
{
    for(auto iter = tasks_data.begin(); iter != tasks_data.end(); ++iter) {
        if (user_id != iter->user_id) {
            continue;
        }

        iter->user_id = 0;
        iter->status = 1;
    }
}
