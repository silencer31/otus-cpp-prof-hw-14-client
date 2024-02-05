#include "adminwindow.h"
#include "ui_adminwindow.h"

// Реакция на клик по строке в таблице с пользователями.
void AdminWindow::user_clicked(const QModelIndex& index)
{
    if (index.row() == 0) {
        clear_fields();
        ui->cbUserType->setCurrentIndex(0);
        return;
    }

    if (data_keeper_ptr->users_is_empty()) {
        return;
    }

    bool ok = false;
    const int user_id = users_table_model->item(index.row(), 0)->data(Qt::DisplayRole).toInt(&ok);

    // Проверка определённого значения.
    if ( !ok || user_id <= 0) {
        show_message(QString("Unexpected id error!\nid: %1").arg(QString::number(user_id)));
        ui->tvUsers->clearSelection();
        return;
    }

    ui->cbUserType->setCurrentIndex(data_keeper_ptr->get_user_data(user_id)->login_type.user_type - 1);

    ui->leLogin->setText( data_keeper_ptr->get_user_data(user_id)->login_type.user_name );
    ui->leSurename->setText( data_keeper_ptr->get_user_data(user_id)->fullname.second );
    ui->leName->setText( data_keeper_ptr->get_user_data(user_id)->fullname.first );
    ui->lePatronymic->setText( data_keeper_ptr->get_user_data(user_id)->fullname.patronymic );
}

// Реакция на клик по строке в таблице с задачами.
void AdminWindow::task_clicked(const QModelIndex& index)
{
    if (index.row() == 0) {
        ui->leDescription->clear();
        ui->tvTasks->clearSelection();
        return;
    }

    if (data_keeper_ptr->tasks_is_empty()) {
        return;
    }

    bool ok = false;
    const int task_id = tasks_table_model->item(index.row(), 0)->data(Qt::DisplayRole).toInt(&ok);

    // Проверка определённого значения.
    if ( !ok || task_id <= 0) {
        show_message(QString("Unexpected id error!\nid: %1").arg(QString::number(task_id)));
        ui->tvTasks->clearSelection();
        return;
    }

    ui->leDescription->setText( data_keeper_ptr->get_task_data(task_id)->description );
}

// Запрос списка пользователей.
void AdminWindow::get_users_list()
{
    // Есть ли связь с сервером.
    if (!request_manager_ptr->connected_to_server()) {
        show_message(QString("No connection with server!"));
        return;
    }

    //Блокируем кнопки
    lock_buttons();

    // Запрос списка id пользователей.
    if ( !handler_ptr->get_users_list()) {
        show_message(QString("Unable to get users list\n%1").arg(handler_ptr->get_error()));
        unlock_buttons();
        return;
    }

    // Очищаем коллекцию с данными пользователей.
    data_keeper_ptr->users_clear();

    // Для всех полученных id запрашиваем логин и тип пользователя.
    for(auto iter = collector_ptr->user_ids_cib(); iter != collector_ptr->user_ids_cie(); ++iter) {
        // Запрос логина и типа пользователя по user id.
        if (!handler_ptr->get_username(*iter)) {
            show_message(QString("Unable to get login and user type\n%1").arg(handler_ptr->get_error()));
            unlock_buttons();
            return;
        }

        // Запрос ФИО пользователя.
        if (!handler_ptr->get_fullname(*iter)) {
            show_message(QString("Unable to get user fullname\n%1").arg(handler_ptr->get_error()));
            unlock_buttons();
            return;
        }

        // Добавляем данные пользователя в коллекцию.
        data_keeper_ptr->add_user( *iter, UserData(collector_ptr->get_login_and_type(), collector_ptr->get_fullname()));
    }

    // Очищаем таблицу от предыдущих строк.
    users_table_model->removeRows(1, users_table_model->rowCount()-1);

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
    }

    // Проверяем, был ли уже получен список с задачами.
    if ( data_keeper_ptr->tasks_is_empty() ) {
        unlock_buttons();
        return;
    }

    bool ok = false;
    int user_id = 0;

    // Если ранее был получен список задач, обновляем столбец с логинами в таблице с задачами.
    for(int i = 1; i < tasks_table_model->rowCount(); ++i) {
        user_id = tasks_table_model->item(i, 5)->data(Qt::DisplayRole).toInt(&ok);

        if (!ok || user_id <= 0) {
            continue;
        }

        tasks_table_model->setData(tasks_table_model->index(i, 6),
                                data_keeper_ptr->users_containes(user_id)
                                ? (data_keeper_ptr->get_user_data(user_id))->login_type.user_name
                                : ((user_id == own_id) ? own_name : QString("Unknown")),
                                Qt::DisplayRole);
    }

    ui->tvUsers->clearSelection();
    unlock_buttons();
}

// Запрос списка задач.
void AdminWindow::get_tasks_list()
{
    // Есть ли связь с сервером.
    if (!request_manager_ptr->connected_to_server()) {
        show_message(QString("No connection with server!"));
        return;
    }

    //Блокируем кнопки
    lock_buttons();

    // Запрос списка id задач.
    bool result = (ui->checkbOnlyOwn->isChecked()
                  ? handler_ptr->get_taskslist(own_id)
                  : handler_ptr->get_taskslist());

    if (!result) {
        show_message(QString("Unable to get tasks list\n%1").arg(handler_ptr->get_error()));
        unlock_buttons();
        return;
    }

    // Очищаем коллекцию с данными задач.
    data_keeper_ptr->tasks_clear();

    // Для всех полученных id запрашиваем данные задачи.
    for(auto iter = collector_ptr->task_ids_cib(); iter != collector_ptr->task_ids_cie(); ++iter) {
        // Запрос данных задачи.
        if (!handler_ptr->get_taskdata(*iter)) {
            show_message(QString("Unable to get taskdata\n%1").arg(handler_ptr->get_error()));
            unlock_buttons();
            return;
        }

        // Добавляем данные задачи в коллекцию.
        data_keeper_ptr->add_task( *iter, collector_ptr->get_task_data());
    }

    int row_number;

    // Очищаем таблицу от предыдущих строк.
    tasks_table_model->removeRows(1, tasks_table_model->rowCount()-1);

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
    }

    ui->tvTasks->clearSelection();
    unlock_buttons();
}

// Создать нового или изменить данные выбранного пользователя.
void AdminWindow::add_or_edit_user()
{
    // Создать нового пользователя.
    if (ui->rbCreateUser->isChecked()) {
        create_user();
        return;
    }

    // Удалить выбранного пользователя.
    if (ui->rbDeleteUser->isChecked()) {
        delete_user();
        return;
    }

    // Изменить тип пользователя.
    if (ui->rbChangeType->isChecked()) {
        change_user_type();
        return;
    }

    // Задать новый пароль.
    if (ui->rbChangePassword->isChecked()) {
        change_password();
        return;
    }
}

// Очистить поля ввода.
void AdminWindow::clear_fields()
{
    ui->leLogin->clear();
    ui->leSurename->clear();
    ui->leName->clear();
    ui->lePatronymic->clear();

    ui->tvUsers->clearSelection();
}

// Отправить запрос на выключение сервера.
void AdminWindow::shutdown_server()
{
    // Есть ли связь с сервером.
    if (!request_manager_ptr->connected_to_server()) {
        show_message(QString("No connection with server!"));
        return;
    }

    // Отправляем запрос на сервер.
    if ( !handler_ptr->shutdown_server()) {
        show_message(QString("Unable to shutdown server!\n%1").arg(handler_ptr->get_error()));
        return;
    }

    request_manager_ptr->disconnect_from_server();

    show_message(QString("Server shutdown completed.\nFurther communication impossible"));
}
