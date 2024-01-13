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
