#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#pragma once

enum class UserType {
    Unsupported = 0,
    Administrator = 1,
    Operator = 2,
    User = 3
};

enum class TaskStatus {
    NotAppointed = 1,
    Appointed = 2,
    InProgress = 3,
    Rejected = 4,
    Completed = 5
};

// Виды команд от клиента к серверу.
enum class CommandType {
    Unknown = 0,	// Неизвестный серверу тип команды.
    Test = 1,		// Тестовый запрос для проверки связи.
    Login = 2,		// Логин пользователя в системе.
    Closedown = 3,	// Сессия будет завершена.
    Shutdown = 4,	// Запрос на выключение сервера.
    Get = 5,		// Получение данных из базы.
    Add = 6,		// Добавление данных в базу.
    Set = 7,		// Изменение значения в базе.
    Del = 8			// Удаление данных из базы.
};

#endif // DATA_TYPES_H
