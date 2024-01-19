#include "operatorwindow.h"
#include "ui_operatorwindow.h"

// Реакция на клик по строке в таблице с пользователями.
void OperatorWindow::user_clicked(const QModelIndex& index)
{
    if (index.row() == 0) {
        ui->leUserId->clear();
        ui->leUserName->clear();
        ui->tvUsers->clearSelection();
        return;
    }

    if (data_keeper_ptr->users_is_empty()) {
        return;
    }

    if (!index_y_user_id_map.contains(index.row())) {
        return;
    }

    const int user_id = index_y_user_id_map[index.row()];

    ui->leUserId->setText( QString::number(user_id) );
    ui->leUserName->setText( data_keeper_ptr->get_user_data(user_id)->login_type.user_name );
}

// Реакция на клик по строке в таблице с задачами.
void OperatorWindow::task_clicked(const QModelIndex& index)
{
    if (index.row() == 0) {
        ui->leTaskName->clear();
        ui->leDeadLine->clear();
        ui->leDescription->clear();
        ui->tvTasks->clearSelection();
        return;
    }

    if (data_keeper_ptr->tasks_is_empty()) {
        return;
    }

    if (!index_y_task_id_map.contains(index.row())) {
        return;
    }

    const int task_id = index_y_task_id_map[index.row()];

    ui->leTaskName->setText( data_keeper_ptr->get_task_data(task_id)->name );
    ui->leDeadLine->setText( data_keeper_ptr->get_task_data(task_id)->deadline );
    ui->leDescription->setText( data_keeper_ptr->get_task_data(task_id)->description );

    ui->cbTaskStatus->setCurrentIndex(data_keeper_ptr->get_task_data(task_id)->status - 1);
}

// Запрос списка пользователей.
void OperatorWindow::get_users_list()
{
    //Блокируем кнопки
    lock_buttons();

    // Есть ли связь с сервером.
    if (!request_manager_ptr->connected_to_server()) {
        message_window_ptr->set_message(QString("No connection with server!"));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Запрос типов пользователей.
    if ( !get_user_types()) {
        unlock_buttons();
        return;
    }

    // Запрос списка id пользователей.
    if (!request_manager_ptr->send_get_userlist()) {
        message_window_ptr->set_message(QString("Unable to send get userlist request!\n\n%1")
                                            .arg(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Get)) {
        message_window_ptr->set_message(QString("Unable to get user ids!\n\n%1").arg(error_text));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Проверяем, что получено от сервера.
    if (!collector_ptr->user_ids_received()) {
        message_window_ptr->set_message(QString("Получена пустая коллекция id пользователей"));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Очищаем коллекцию с данными пользователей.
    data_keeper_ptr->users_clear();

    // Для всех полученных id запрашиваем логин и тип пользователя.
    for(auto iter = collector_ptr->user_ids_cib(); iter != collector_ptr->user_ids_cie(); ++iter) {
        // Запрос логина и типа пользователя.
        if (!request_manager_ptr->send_get_username(*iter)) {
            message_window_ptr->set_message(QString("Unable to send get username request!\n\n%1")
                                                .arg(request_manager_ptr->get_last_error()));
            message_window_ptr->exec();
            unlock_buttons();
            return;
        }

        // Контроль выполнения запроса.
        if ( !handle_request(CommandType::Get)) {
            message_window_ptr->set_message(QString("Unable to get user login and type!\n\n%1").arg(error_text));
            message_window_ptr->exec();
            unlock_buttons();
            return;
        }

        // Запрос ФИО пользователя.
        if (!request_manager_ptr->send_get_fullname(*iter)) {
            message_window_ptr->set_message(QString("Unable to send get fullname request!\n\n%1")
                                                .arg(request_manager_ptr->get_last_error()));
            message_window_ptr->exec();
            unlock_buttons();
            return;
        }

        // Контроль выполнения запроса.
        if ( !handle_request(CommandType::Get)) {
            message_window_ptr->set_message(QString("Unable to get user fullname!\n\n%1").arg(error_text));
            message_window_ptr->exec();
            unlock_buttons();
            return;
        }

        // Добавляем данные пользователя в коллекцию.
        data_keeper_ptr->add_user( *iter, UserData(collector_ptr->get_login_and_type(), collector_ptr->get_fullname()));
    }

    // Очищаем таблицу от предыдущих строк.
    users_table_model->removeRows(1, users_table_model->rowCount()-1);

    // Удаляем все связи между строками таблицы и id пользователя.
    index_y_user_id_map.clear();

    int row_number;

    // Вывод данных в таблицу.
    for(auto iter = data_keeper_ptr->users_data_cib(); iter != data_keeper_ptr->users_data_cie(); ++iter) {
        users_table_model->insertRow(users_table_model->rowCount());
        row_number = users_table_model->rowCount()-1;

        users_table_model->setData(users_table_model->index(row_number, 0), QString::number(iter.key()), Qt::DisplayRole);
        users_table_model->setData(users_table_model->index(row_number, 1), iter->login_type.user_name, Qt::DisplayRole);
        users_table_model->setData(users_table_model->index(row_number, 2), collector_ptr->type_description(iter->login_type.user_type), Qt::DisplayRole);
        users_table_model->setData(users_table_model->index(row_number, 3), iter->fullname.second, Qt::DisplayRole);
        users_table_model->setData(users_table_model->index(row_number, 4), iter->fullname.first, Qt::DisplayRole);
        users_table_model->setData(users_table_model->index(row_number, 5), iter->fullname.patronymic, Qt::DisplayRole);

        index_y_user_id_map[row_number] = iter.key();
    }

    unlock_buttons();
}

// Запрос списка задач.
void OperatorWindow::get_tasks_list()
{
    QModelIndexList selection = ui->tvUsers->selectionModel()->selectedRows();

    if (ui->rbChoosen->isChecked()) {
        if (selection.isEmpty()) {
            message_window_ptr->set_message(QString("Choose a user to request tasks for"));
            message_window_ptr->exec();
            return;
        }

        if (!index_y_user_id_map.contains(selection.at(0).row())) {
            message_window_ptr->set_message(QString("Unexpected index error!"));
            message_window_ptr->exec();
            return;
        }
    }

    // Блокируем кнопки
    lock_buttons();

    // Есть ли связь с сервером.
    if (!request_manager_ptr->connected_to_server()) {
        message_window_ptr->set_message(QString("No connection with server!"));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Запрос типов пользователей.
    if ( !get_user_types()) {
        unlock_buttons();
        return;
    }

    // Запрос возможных статусов задач.
    if ( !get_task_statuses()) {
        unlock_buttons();
        return;
    }

    // Запрос списка id задач.
    bool result = (ui->rbChoosen->isChecked()
                  ? request_manager_ptr->send_get_tasklist(index_y_user_id_map[selection.at(0).row()])
                  : ( ui->rbOnlyOwn->isChecked()
                              ? request_manager_ptr->send_get_tasklist(own_id)
                              : request_manager_ptr->send_get_tasklist())
                   );

    if (!result) {
        message_window_ptr->set_message(QString("Unable to send get tasklist request!\n\n%1")
                                            .arg(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Get)) {
        message_window_ptr->set_message(QString("Unable to get task ids!\n\n%1").arg(error_text));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Проверяем, что получено от сервера.
    if (!collector_ptr->task_ids_received()) {
        message_window_ptr->set_message(QString("Получена пустая коллекция id задач"));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Очищаем коллекцию с данными задач.
    data_keeper_ptr->tasks_clear();

    // Для всех полученных id запрашиваем данные задачи.
    for(auto iter = collector_ptr->task_ids_cib(); iter != collector_ptr->task_ids_cie(); ++iter) {
        // Запрос данных задачи.
        if (!request_manager_ptr->send_get_taskdata(*iter)) {
            message_window_ptr->set_message(QString("Unable to send get taskdata request!\n\n%1")
                                                .arg(request_manager_ptr->get_last_error()));
            message_window_ptr->exec();
            unlock_buttons();
            return;
        }

        // Контроль выполнения запроса.
        if ( !handle_request(CommandType::Get)) {
            message_window_ptr->set_message(QString("Unable to get task data!\n\n%1").arg(error_text));
            message_window_ptr->exec();
            unlock_buttons();
            return;
        }

        // Добавляем данные задачи в коллекцию.
        data_keeper_ptr->add_task( *iter, collector_ptr->get_task_data());
    }

    // Очищаем таблицу от предыдущих строк.
    tasks_table_model->removeRows(1, tasks_table_model->rowCount()-1);

    // Очищаем коллекцию связей id задачи и номера строки в таблице.
    index_y_task_id_map.clear();

    int row_number;

    // Вывод данных в таблицу.
    for(auto iter = data_keeper_ptr->tasks_data_cib(); iter != data_keeper_ptr->tasks_data_cie(); ++iter) {
        tasks_table_model->insertRow(tasks_table_model->rowCount());
        row_number = tasks_table_model->rowCount()-1;

        tasks_table_model->setData(tasks_table_model->index(row_number, 0), iter.key(), Qt::DisplayRole);
        tasks_table_model->setData(tasks_table_model->index(row_number, 1), collector_ptr->status_description(iter->status), Qt::DisplayRole);
        tasks_table_model->setData(tasks_table_model->index(row_number, 2), iter->name, Qt::DisplayRole);
        tasks_table_model->setData(tasks_table_model->index(row_number, 3), iter->deadline, Qt::DisplayRole);
        tasks_table_model->setData(tasks_table_model->index(row_number, 4), iter->description, Qt::DisplayRole);
        tasks_table_model->setData(tasks_table_model->index(row_number, 5), QString::number(iter->user_id), Qt::DisplayRole);

        tasks_table_model->setData(tasks_table_model->index(row_number, 6),
                                   data_keeper_ptr->users_containes(iter->user_id)
                                       ? (data_keeper_ptr->get_user_data(iter->user_id))->login_type.user_name
                                       : ((iter->user_id == own_id) ? own_name : QString("Unknown")),
                                   Qt::DisplayRole);

        index_y_task_id_map[row_number] = iter.key();
    }

    unlock_buttons();
}

// Создать новую или изменить данные выбранной задачи.
void OperatorWindow::add_or_edit_task()
{
    // Создать новую задачу.
    if (ui->rbCreateTask->isChecked()) {
        create_task();
        return;
    }

    // Удалить выбранную задачу.
    if (ui->rbDeleteTask->isChecked()) {
        delete_task();
        return;
    }

    // Изменить статус задачи.
    if (ui->rbChangeStatus->isChecked()) {
        change_task_status();
        return;
    }

    // Задать новую дату, до которой д/б выполнена задача.
    if (ui->rbSetDeadline->isChecked()) {
        set_task_deadline();
        return;
    }

    // Назначить другого пользователя на задачу.
    if (ui->rbAppoint->isChecked()) {
        appoint_user();
        return;
    }

    message_window_ptr->set_message(QString("Choose an action"));
    message_window_ptr->exec();
}

// Очистить поля ввода.
void OperatorWindow::clear_fields()
{
    ui->leTaskName->clear();
    ui->leDeadLine->clear();
    ui->leDescription->clear();
    ui->leUserId->clear();
    ui->leUserName->clear();

    ui->tvUsers->clearSelection();
    ui->tvTasks->clearSelection();
}
