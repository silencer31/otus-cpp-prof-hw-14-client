#ifndef EXTRA_FUNCTIONS_H
#define EXTRA_FUNCTIONS_H

#pragma once

#include <QString>

// Функция проверяет, что введён корректный ip адрес.
bool check_address(const QString& address_value);

// Функция проверяет и возвращает числовое значение номера порта.
int get_port(const QString& port_value);

#endif // EXTRA_FUNCTIONS_H
