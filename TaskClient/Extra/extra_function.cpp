#include "extra_functions.h"

#include <QStringList>

// Функция проверяет, что введён корректный ip адрес.
bool check_address(const QString& address_value)
{
    const QStringList tmp_list = address_value.split('.');
    if (tmp_list.size() != 4) {
        return false;
    }

    bool res = false;
    int part_val = 0;

    foreach (const QString& addr_part, tmp_list) {
        res = false;
        part_val = addr_part.toInt(&res);

        if (!res || part_val < 0 || part_val > 255) {
            return false;
        }
    }

    return true;
}

// Функция проверяет и возвращает числовое значение номера порта.
int get_port(const QString& port_value)
{
    bool res = false;
    int port = port_value.toInt(&res);

    return (res ? port : -1); // Если преобразовать не удалось, возвращаем -1.
}
