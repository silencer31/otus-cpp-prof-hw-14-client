#include "operatorwindow.h"
#include "ui_operatorwindow.h"

// Запрос возможных типов пользователей и статусов задач.
bool OperatorWindow::get_user_types()
{
    // Проверяем, был ли ранее получен список типов пользователей.
    if (collector_ptr->user_types_received()) {
        return true;
    }

    // Запрос списка типа пользователей.
    if (!request_manager_ptr->send_get_typelist()) {
        message_window_ptr->set_message(QString("Unable to send get typelist request!\n\n%1")
                                            .arg(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return false;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Get)) {
        message_window_ptr->set_message(QString("Unable to get user types!\n\n%1").arg(error_text));
        message_window_ptr->exec();
        return false;
    }

    // Проверяем, что получено от сервера.
    if (!collector_ptr->user_types_received()) {
        message_window_ptr->set_message(QString("Получена пустая коллекция типов пользователей"));
        message_window_ptr->exec();
        return false;
    }

    return true;
}

// Запрос возможных статусов задач.
bool OperatorWindow::get_task_statuses()
{
    // Проверяем, был ли ранее получен список статусов задач.
    if (collector_ptr->task_statuses_received()) {
        return true;
    }

    // Запрос списка статусов задач.
    if (!request_manager_ptr->send_get_statuslist()) {
        message_window_ptr->set_message(QString("Unable to send get statuslist request!\n\n%1")
                                            .arg(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return false;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Get)) {
        message_window_ptr->set_message(QString("Unable to get task statuses!\n\n%1").arg(error_text));
        message_window_ptr->exec();
        return false;
    }

    // Проверяем, что получено от сервера.
    if (!collector_ptr->task_statuses_received()) {
        message_window_ptr->set_message(QString("Получена пустая коллекция статусов задач"));
        message_window_ptr->exec();
        return false;
    }

    // Обновляем список доступных статусов в выпадающем меню.
    ui->cbTaskStatus->clear();

    for(auto iter = collector_ptr->task_statuses_cib(); iter != collector_ptr->task_statuses_cie(); ++iter) {
        ui->cbTaskStatus->addItem(QString("%1 : %2").arg(QString::number(iter.key()), iter.value()));
    }

    return true;
}

// Создать новую задачу.
void OperatorWindow::create_task()
{
    const QString task_name   = ui->leTaskName->text();
    const QString deadline    = ui->leDeadLine->text();
    const QString description = ui->leDescription->text();

    // Проверка, заполнены ли поля.
    if (task_name.isEmpty() || deadline.isEmpty() || description.isEmpty()) {
        message_window_ptr->set_message(QString("All the fields have to be filled in!"));
        message_window_ptr->exec();
        return;
    }

    // Проверка, используется ли уже такое имя задачи.
    if (data_keeper_ptr->task_name_received(task_name)) {
        message_window_ptr->set_message(QString("Task name %1 is already in use!\nPlease enter another value").arg(task_name));
        message_window_ptr->exec();
        return;
    }

    // Должен быть выбран пользователь.
    QModelIndexList selection = ui->tvUsers->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        message_window_ptr->set_message(QString("Choose a user for a new task"));
        message_window_ptr->exec();
        return;
    }

    if (!index_y_user_id_map.contains(selection.at(0).row())) {
        message_window_ptr->set_message(QString("Unexpected users index error!"));
        message_window_ptr->exec();
        return;
    }

    const int user_id = index_y_user_id_map[selection.at(0).row()];

    // Отправляем запрос на создание новой задачи.
    if ( !request_manager_ptr->send_add_task(user_id, deadline, task_name, description) ) {
        message_window_ptr->set_message(request_manager_ptr->get_last_error());
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Add)) {
        message_window_ptr->set_message(QString("Unable to add new task!\n\n%1").arg(error_text));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Добавляем новую задачу в коллекцию данных задач.
    const TaskData task_data{user_id, 2, deadline, task_name, description};
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
    tasks_table_model->setData(tasks_table_model->index(row_number, 5), QString::number(user_id), Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(row_number, 6), data_keeper_ptr->get_user_data(user_id)->login_type.user_name, Qt::DisplayRole);

    index_y_task_id_map[row_number] = new_id;

    message_window_ptr->set_message(QString("Task %1\nhas been successfully created").arg(task_name));
    message_window_ptr->exec();
    unlock_buttons();
}

// Удалить выбранную задачу.
void OperatorWindow::delete_task()
{
    // Должна быть выбрана задача.
    QModelIndexList selection = ui->tvTasks->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        message_window_ptr->set_message(QString("Choose a task to delete"));
        message_window_ptr->exec();
        return;
    }

    if (!index_y_task_id_map.contains(selection.at(0).row())) {
        message_window_ptr->set_message(QString("Unexpected tasks index error!"));
        message_window_ptr->exec();
        return;
    }

    const int task_id = index_y_task_id_map[selection.at(0).row()];
    const QString task_to_del = data_keeper_ptr->get_task_data(task_id)->name;

    lock_buttons();

    // Отправляем запрос на удаление задачи.
    if ( !request_manager_ptr->send_del_task(task_id) ) {
        message_window_ptr->set_message(request_manager_ptr->get_last_error());
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Del)) {
        message_window_ptr->set_message(QString("Unable to delete task\n%1!\n\n%2")
                                            .arg(task_to_del, error_text));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Убираем задачу из таблицы.
    tasks_table_model->removeRows(selection.at(0).row(), 1);
    ui->tvTasks->clearSelection();

    // Убираем связь между id и номером строки в таблице.
    index_y_task_id_map.remove(task_id);

    // Удаляем данные задачи.
    data_keeper_ptr->del_task_data(task_id);


    message_window_ptr->set_message(QString("Task %1\nhas been successfully deleted").arg(task_to_del));
    message_window_ptr->exec();
    unlock_buttons();
}

// Изменить статус задачи.
void OperatorWindow::change_task_status()
{
    // Должна быть выбрана задача.
    QModelIndexList selection = ui->tvTasks->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        message_window_ptr->set_message(QString("Choose a task to change status"));
        message_window_ptr->exec();
        return;
    }

    if (!index_y_task_id_map.contains(selection.at(0).row())) {
        message_window_ptr->set_message(QString("Unexpected tasks index error!"));
        message_window_ptr->exec();
        return;
    }

    const int task_id = index_y_task_id_map[selection.at(0).row()];
    const QString task_name = data_keeper_ptr->get_task_data(task_id)->name;

    const int status = ui->cbTaskStatus->currentIndex()+1;

    // Проверяем чтобы текущий и новый статус не совпадали.
    if (status == data_keeper_ptr->get_task_data(task_id)->status) {
        message_window_ptr->set_message(QString("Old and new task statuses are the same"));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    lock_buttons();

    // Отправляем запрос на изменение статуса задачи.
    if ( !request_manager_ptr->send_set_taskstatus(task_id, status) ) {
        message_window_ptr->set_message(request_manager_ptr->get_last_error());
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Set)) {
        message_window_ptr->set_message(QString("Unable to change status for\ntask %1!\n\n%2")
                                            .arg(task_name, error_text));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Запоминаем новый статус задачи.
    data_keeper_ptr->set_task_status(task_id, status);

    // Обновляем содержимое ячейки в таблице.
    tasks_table_model->setData(tasks_table_model->index(selection.at(0).row(), 1), collector_ptr->status_description(status), Qt::DisplayRole);

    message_window_ptr->set_message(QString("Task status has been successfully changed\nto")
                                        .arg(collector_ptr->status_description(status)));
    message_window_ptr->exec();
    unlock_buttons();
}

// Задать новую дату, до которой д/б выполнена задача.
void OperatorWindow::set_task_deadline()
{

}

// Назначить другого пользователя на задачу.
void OperatorWindow::appoint_user()
{

}
