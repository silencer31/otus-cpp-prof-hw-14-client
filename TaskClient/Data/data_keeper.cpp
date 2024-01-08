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
