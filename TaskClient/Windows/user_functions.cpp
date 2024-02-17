#include "userwindow.h"
#include "ui_userwindow.h"

// Создать задачу для себя.
void UserWindow::create_task()
{
    const QString task_name   = ui->leTaskName->text();
    const QString deadline    = ui->leDeadLine->text();
    const QString description = ui->leDescription->text();

    // Проверка, заполнены ли поля.
    if (task_name.isEmpty() || deadline.isEmpty() || description.isEmpty()) {
        show_message(QString("All the fields have to be filled in!"));
        return;
    }

    // Проверка, используется ли уже такое имя задачи.
    if (data_keeper_ptr->task_name_received(task_name)) {
        show_message(QString("Task name %1 is already in use!\nPlease enter another value").arg(task_name));
        return;
    }

    lock_buttons();

    // Отправляем запрос на создание новой задачи.
    if ( !handler_ptr->create_task( data_keeper_ptr->get_own_id(), deadline, task_name, description) ) {
        show_message(QString("Unable to add new task\n%1").arg(handler_ptr->get_error()));
        unlock_buttons();
        return;
    }

    // Добавляем новую задачу в коллекцию данных задач.
    const TaskData task_data{data_keeper_ptr->get_own_id(), 2, deadline, task_name, description};
    const int new_id = collector_ptr->get_item_id();

    data_keeper_ptr->add_task(new_id, task_data);

    // Добавляем новую задачу в таблицу.
    tasks_table_model->insertRow(tasks_table_model->rowCount());

    const int row_number = tasks_table_model->rowCount()-1;

    tasks_table_model->setData(tasks_table_model->index(row_number, 0), new_id, Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(row_number, 1), collector_ptr->status_description(2), Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(row_number, 2), task_name, Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(row_number, 3), deadline, Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(row_number, 4), description, Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(row_number, 5), QString::number(data_keeper_ptr->get_own_id()), Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(row_number, 6), data_keeper_ptr->get_own_login(), Qt::DisplayRole);

    show_message(QString("Task %1\nhas been successfully created").arg(task_name));

    unlock_buttons();
}

// Забрать чужую задачу себе.
void UserWindow::take_task()
{
    // Должна быть выбрана задача.
    const QModelIndexList selection = ui->tvTasks->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        show_message(QString("Choose a task to take"));
        return;
    }

    bool ok = false;
    const int task_id = tasks_table_model->item(selection.at(0).row())->data(Qt::DisplayRole).toInt(&ok);

    if (!ok || task_id <= 0) {
        return;
    }

    ok = false;
    const int user_id = tasks_table_model->item(selection.at(0).row(), 5)->data(Qt::DisplayRole).toInt(&ok);

    if (!ok || user_id < 0) {
        return;
    }

    // Проверяем, назначена ли уже выбранная задача на текущего пользователя.
    if (user_id == data_keeper_ptr->get_own_id()) {
        show_message(QString("This task is already appoited to you"));
        return;
    }

    const QString task_to_take = data_keeper_ptr->get_task_data(task_id)->name;

    lock_buttons();

    // Устанавливаем себя как исполнителя задачи.
    if ( !handler_ptr->appoint_task_user(task_id, data_keeper_ptr->get_own_id()) ) {
        show_message(QString("Unable to take task\n%1\n%2")
                         .arg(task_to_take, handler_ptr->get_error()));
        unlock_buttons();
        return;
    }

    // Запоминаем новое значение user_id исполнителя задачи.
    data_keeper_ptr->set_task_user(task_id, data_keeper_ptr->get_own_id());

    // Проверяем, был ли у задачи статус "not appointed".
    if (data_keeper_ptr->get_task_data(task_id)->status == 1) {
        // Устанавливаем статус задачи "appointed".
        data_keeper_ptr->set_task_status(task_id, 2);

        // Обновляем содержимое ячейки в таблице.
        tasks_table_model->setData(tasks_table_model->index(selection.at(0).row(), 1),
                                   collector_ptr->status_description(2), Qt::DisplayRole);

        ui->cbTaskStatus->setCurrentIndex(1);
    }

    // Обновляем содержимое ячейки в таблице.
    tasks_table_model->setData(tasks_table_model->index(selection.at(0).row(), 5), QString::number(data_keeper_ptr->get_own_id()), Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(selection.at(0).row(), 6), data_keeper_ptr->get_own_login(), Qt::DisplayRole);

    show_message(QString("Task %1\nhas been successfully taken").arg(task_to_take));

    unlock_buttons();
}

// Изменить статус своей задачи.
void UserWindow::change_task_status()
{
    // Должна быть выбрана задача.
    const QModelIndexList selection = ui->tvTasks->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        show_message(QString("Choose a task to change status"));
        return;
    }

    bool ok = false;
    const int task_id = tasks_table_model->item(selection.at(0).row())->data(Qt::DisplayRole).toInt(&ok);

    if (!ok || task_id <= 0) {
        return;
    }

    ok = false;
    const int user_id = tasks_table_model->item(selection.at(0).row(), 5)->data(Qt::DisplayRole).toInt(&ok);

    if (!ok || user_id < 0) {
        return;
    }

    // Пользователь может изменять статус только одной из своих задач.
    if (user_id != data_keeper_ptr->get_own_id()) {
        show_message(QString("Take task before changing status"));
        return;
    }

    const QString task_name = data_keeper_ptr->get_task_data(task_id)->name;

    const int status = ui->cbTaskStatus->currentIndex()+1;

    // Пользователь не может отказаться от своей задачи.
    if (status == 1) {
        show_message(QString("You are not allowed to abandon the task"));
        unlock_buttons();
        return;
    }

    // Проверяем чтобы текущий и новый статус не совпадали.
    if (status == data_keeper_ptr->get_task_data(task_id)->status) {
        show_message(QString("Old and new task statuses are the same"));
        unlock_buttons();
        return;
    }

    lock_buttons();

    // Измененяем статус задачи.
    if ( !handler_ptr->change_task_status(task_id, status) ) {
        show_message(QString("Unable to change status for task\n%1\n%2")
                         .arg(task_name, handler_ptr->get_error()));
        unlock_buttons();
        return;
    }

    // Запоминаем новый статус задачи.
    data_keeper_ptr->set_task_status(task_id, status);

    // Обновляем содержимое ячейки в таблице.
    tasks_table_model->setData(tasks_table_model->index(selection.at(0).row(), 1),
                               collector_ptr->status_description(status), Qt::DisplayRole);

    show_message(QString("Task status has been successfully changed\nto %1")
                     .arg(collector_ptr->status_description(status)));

    unlock_buttons();
}

// Задать новую дату, до которой д/б выполнена своя задача.
void UserWindow::set_task_deadline()
{
    // Должна быть выбрана задача.
    const QModelIndexList selection = ui->tvTasks->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        show_message(QString("Choose a task to change deadline"));
        return;
    }

    bool ok = false;
    const int task_id = tasks_table_model->item(selection.at(0).row())->data(Qt::DisplayRole).toInt(&ok);

    if (!ok || task_id <= 0) {
        return;
    }

    ok = false;
    const int user_id = tasks_table_model->item(selection.at(0).row(), 5)->data(Qt::DisplayRole).toInt(&ok);

    if (!ok || user_id < 0) {
        return;
    }

    // Пользователь может изменять deadline только одной из своих задач.
    if (user_id != data_keeper_ptr->get_own_id()) {
        show_message(QString("Take task before changing deadline"));
        return;
    }

    const QString task_name = data_keeper_ptr->get_task_data(task_id)->name;
    const QString deadline = ui->leDeadLine->text();

    if (deadline.isEmpty()) {
        show_message(QString("Deadline field must be filled in"));
        return;
    }

    // Сравниваем старое значение deadline и новое.
    if (deadline == data_keeper_ptr->get_task_data(task_id)->deadline) {
        show_message(QString("Old and new deadline values are equal"));
        return;
    }

    lock_buttons();

    // Отправляем запрос на изменение deadline задачи.
    if ( !handler_ptr->set_task_deadline(task_id, deadline) ) {
        show_message(QString("Unable to change deadline for task\n%1\n%2")
                        .arg(task_name, handler_ptr->get_error()));
        unlock_buttons();
        return;
    }

    // Запоминаем новое значение deadline задачи.
    data_keeper_ptr->set_task_deadline(task_id, deadline);

    // Обновляем содержимое ячейки в таблице.
    tasks_table_model->setData(tasks_table_model->index(selection.at(0).row(), 3), deadline, Qt::DisplayRole);

    show_message(QString("Task deadline has been successfully changed\nto %1").arg(deadline));

    unlock_buttons();
}
