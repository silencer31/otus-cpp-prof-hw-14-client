#ifndef DATA_KEEPER_H
#define DATA_KEEPER_H

#pragma once

#include "data_collections.h"

#include <memory>

/***********
 *
 *  Этот класс хранит коллекции данных о пользователях и задачах.
 *
 * ********/

class DataKeeper
{
public:
    DataKeeper() = default;
    ~DataKeeper() = default;

    /// Очистка коллекций.
    // Очистка всех данных о пользователях.
    void users_clear() {
        users_data.clear();
    }

    // Очистка всех данных о задачах.
    void tasks_clear() {
        tasks_data.clear();
    }

    /// Добавление данных в коллекции.
    // Добавление данных пользователя.
    void add_user(const int id, const UserData& user) {
        users_data[id] = user;
    }

    // Добавление данных задачи.
    void add_task(const int id, const TaskData& task) {
        tasks_data[id] = task;
    }

    /// Получение данных.
    // Узнать, пуста ли коллекция с пользователями.
    bool users_is_empty() {
        return users_data.isEmpty();
    }

    // Узнать, пуста ли коллекция с задачами.
    bool tasks_is_empty() {
        return tasks_data.isEmpty();
    }

    // Узнать, есть ли пользователь с таким id в коллекции c данными пользователей.
    bool users_containes(const int id) {
        return users_data.contains(id);
    }

    // Узнать, есть ли задача с таким id в коллекции c данными задач.
    bool tasks_containes(const int id) {
        return tasks_data.contains(id);
    }

    // Узнать, есть ли уже такой логин среди полученных.
    bool user_name_received(const QString& login);

    // Узнать, есть ли уже такая задача среди полученных по имени задачи.
    bool task_name_received(const QString& task_name);

    // Получить id пользователя - исполнителя задачи.
    int get_task_user(const int task_id) {
        return (tasks_data.contains(task_id) ? tasks_data[task_id].user_id : -1);
    }

    /// Удаление данных из коллекций.
    // Удалить данные пользователя по его id.
    void del_user_data(const int id) {
        users_data.remove(id);
    }

    // Удалить данные задачи по её id.
    void del_task_data(const int id) {
        tasks_data.remove(id);
    }

    /// Итераторы на коллекцию с данными пользователей.
    // Получить итератор на начало коллекции с данными пользователей.
    QMap<int, UserData>::const_iterator users_data_cib() {
        return users_data.constBegin();
    }

    // Получить итератор на конец коллекции с данными пользователей.
    QMap<int, UserData>::const_iterator users_data_cie() {
        return users_data.constEnd();
    }

    // Получить итератор на данные пользователя по его id.
    QMap<int, UserData>::const_iterator get_user_data(const int id) {
        return users_data.constFind(id);
    }

    /// Итераторы на коллекцию с данными задач.
    // Получить итератор на начало коллекции с данными задач.
    QMap<int, TaskData>::const_iterator tasks_data_cib() {
        return tasks_data.constBegin();
    }

    // Получить итератор на конец коллекции с данными задач.
    QMap<int, TaskData>::const_iterator tasks_data_cie() {
        return tasks_data.constEnd();
    }

    // Получить итератор на данные задачи по её id.
    const QMap<int, TaskData>::const_iterator get_task_data(const int id) {
        return tasks_data.constFind(id);
    }

    /// Корректировка полученных данных.
    // Изменить тип пользователя.
    void set_user_type(const int user_id, const int user_type) {
        users_data[user_id].login_type.user_type = user_type;
    }

    // Изменить статус задачи.
    void set_task_status(const int task_id, const int status) {
        tasks_data[task_id].status = status;
    }

    // Изменить deadline задачи.
    void set_task_deadline(const int task_id, const QString& deadline) {
        tasks_data[task_id].deadline = deadline;
    }

    // Назначение пользователя на задачу.
    void set_task_user(const int task_id, const int user_id) {
        tasks_data[task_id].user_id = user_id;
    }

    // Сброс назначенного пользователя у задачи.
    void reset_task_user(const int task_id) {
        tasks_data[task_id].user_id = 0;
    }

    // Для всех задач, на которые назначен переданный user_id установить user_id = 0 и сбросить статус на Not appointed.
    // Используется при удалении пользователя с переданным user_id.
    void reset_tasks_status(const int user_id);

private:
    QMap<int, UserData> users_data;
    QMap<int, TaskData> tasks_data;
};

using data_keeper_shared = std::shared_ptr<DataKeeper>;

#endif // DATA_KEEPER_H
