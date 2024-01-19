#include "adminwindow.h"
#include "ui_adminwindow.h"

// Запрос возможных типов пользователей и статусов задач.
bool AdminWindow::get_user_types()
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


    // Обновляем список доступных типов в выпадающем меню.
    ui->cbUserType->clear();

    for(auto iter = collector_ptr->user_types_cib(); iter != collector_ptr->user_types_cie(); ++iter) {
        ui->cbUserType->addItem(QString("%1 : %2").arg(QString::number(iter.key()), iter.value()));
    }

    return true;
}

// Запрос возможных статусов задач.
bool AdminWindow::get_task_statuses()
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
    if (collector_ptr->task_statuses_received()) {
        return true;
    }

    message_window_ptr->set_message(QString("Получена пустая коллекция статусов задач"));
    message_window_ptr->exec();
    return false;
}

// Запрос на создание нового пользователя.
void AdminWindow::create_user()
{
    const QString user_name  = ui->leLogin->text();
    const QString surename   = ui->leSurename->text();
    const QString name       = ui->leName->text();
    const QString patronymic = ui->lePatronymic->text();

    // Проверка, заполнены ли поля.
    if (user_name.isEmpty() || surename.isEmpty() || name.isEmpty() || patronymic.isEmpty()) {
        message_window_ptr->set_message(QString("All the fields have to be filled in!"));
        message_window_ptr->exec();
        return;
    }

    // Проверка, используется ли уже такой логин.
    if (data_keeper_ptr->user_name_received(user_name)) {
        message_window_ptr->set_message(QString("Login %1 is already in use!\nPlease enter another value").arg(user_name));
        message_window_ptr->exec();
        return;
    }

    // Запрашиваем пароль для нового пользователя.
    passwd_window_ptr->clear_fields();
    if ( passwd_window_ptr->exec() == QDialog::Rejected ) {
        return;
    }

    const QString pass_value_1 = passwd_window_ptr->get_first_value();
    const QString pass_value_2 = passwd_window_ptr->get_second_value();

    // Не должно быть пустого значения пароля.
    if (pass_value_1.isEmpty() || pass_value_2.isEmpty()) {
        message_window_ptr->set_message(QString("Both password fields have to be filled in!"));
        message_window_ptr->exec();
        return;
    }

    // Оба значения должны совпадать.
    if (pass_value_1 != pass_value_2) {
        message_window_ptr->set_message(QString("Password values do not match!"));
        message_window_ptr->exec();
        return;
    }

    const int user_type = ui->cbUserType->currentIndex()+1;

    lock_buttons();

    // Отправляем запрос на создание нового пользователя.
    if ( !request_manager_ptr->send_add_user(user_name, user_type, pass_value_1,
                                             surename, name, patronymic) ) {
        message_window_ptr->set_message(request_manager_ptr->get_last_error());
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Add)) {
        message_window_ptr->set_message(QString("Unable to add new user!\n\n%1").arg(error_text));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Добавляем нового пользователя в коллекцию данных пользователей.
    const UserData user_data{ LoginType(user_name, user_type), Fullname(surename, name, patronymic) };
    const int new_id = collector_ptr->get_item_id();

    data_keeper_ptr->add_user(new_id, user_data);

    // Добавляем нового пользователя в таблицу.
    users_table_model->insertRow(users_table_model->rowCount());
    const int row_number = users_table_model->rowCount()-1;

    users_table_model->setData(users_table_model->index(row_number, 0), QString::number(new_id), Qt::DisplayRole);
    users_table_model->setData(users_table_model->index(row_number, 1), user_name, Qt::DisplayRole);
    users_table_model->setData(users_table_model->index(row_number, 2), collector_ptr->type_description(user_type), Qt::DisplayRole);
    users_table_model->setData(users_table_model->index(row_number, 3), surename, Qt::DisplayRole);
    users_table_model->setData(users_table_model->index(row_number, 4), name, Qt::DisplayRole);
    users_table_model->setData(users_table_model->index(row_number, 5), patronymic, Qt::DisplayRole);

    index_y_user_id_map[row_number] = new_id;

    message_window_ptr->set_message(QString("User %1 has been successfully created").arg(user_name));
    message_window_ptr->exec();
    unlock_buttons();
}

// Удалить выбранного пользователя.
void AdminWindow::delete_user()
{
    QModelIndexList selection = ui->tvUsers->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        message_window_ptr->set_message(QString("Choose a user to delete"));
        message_window_ptr->exec();
        return;
    }

    if (!index_y_user_id_map.contains(selection.at(0).row())) {
        message_window_ptr->set_message(QString("Unexpected index error!"));
        message_window_ptr->exec();
        return;
    }

    const int user_id = index_y_user_id_map[selection.at(0).row()];

    // Проверяем, нет ли попытки удалить себя.
    if (user_id == own_id) {
        message_window_ptr->set_message(QString("You are not able to delete yourself!"));
        message_window_ptr->exec();
        return;
    }

    const QString user_to_del = data_keeper_ptr->get_user_data(user_id)->login_type.user_name;

    lock_buttons();

    // При удалении пользователя, все задачи, которые были на него назначены должны перейти в статус "not appointed".

    // Отправляем запрос на удаление пользователя.
    if ( !request_manager_ptr->send_del_user(user_id) ) {
        message_window_ptr->set_message(request_manager_ptr->get_last_error());
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Del)) {
        message_window_ptr->set_message(QString("Unable to delete user %1!\n\n%2").arg(user_to_del, error_text));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Убираем пользователя из таблицы.
    users_table_model->removeRows(selection.at(0).row(), 1);
    ui->tvUsers->clearSelection();

    // Корректируем связи между номером строки и id.
    for(auto iter = index_y_user_id_map.find(user_id) ; iter != (index_y_user_id_map.end() - 1); ++iter ) {
        *iter = (iter + 1).value();
    }

    // Убираем связь между id и номером строки в таблице.
    index_y_user_id_map.remove(user_id);

    // Удаляем данные пользователя.
    data_keeper_ptr->del_user_data(user_id);

    // Изменяем отображение в таблице.
    for(auto iter = index_y_task_id_map.cbegin(); iter != index_y_task_id_map.cend(); ++iter) {
        if (data_keeper_ptr->get_task_data(iter.value())->user_id != user_id) {
            continue;
        }

        tasks_table_model->setData(tasks_table_model->index(iter.key(), 1), collector_ptr->status_description(1), Qt::DisplayRole);
        tasks_table_model->setData(tasks_table_model->index(iter.key(), 5), QString("0"), Qt::DisplayRole);
        tasks_table_model->setData(tasks_table_model->index(iter.key(), 6), QString("No user"), Qt::DisplayRole);
    }

    // Всем задачам, на которые был назначен пользователь ставим статус "Not appointed" и user_id = 0.
    data_keeper_ptr->reset_tasks_status(user_id);

    message_window_ptr->set_message(QString("User %1 has been successfully deleted").arg(user_to_del));
    message_window_ptr->exec();
    unlock_buttons();
}

// Запрос на изменение данных пользователя.
void AdminWindow::change_user_type()
{
    QModelIndexList selection = ui->tvUsers->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        message_window_ptr->set_message(QString("Choose a user to change user type"));
        message_window_ptr->exec();
        return;
    }

    if (!index_y_user_id_map.contains(selection.at(0).row())) {
        message_window_ptr->set_message(QString("Unexpected index error!"));
        message_window_ptr->exec();
        return;
    }

    const int user_id = index_y_user_id_map[selection.at(0).row()];

    if (!data_keeper_ptr->users_containes(user_id)) {
        return;
    }

    // Проверяем, нет ли попытки изменить свой тип.
    if (user_id == own_id) {
        message_window_ptr->set_message(QString("You are not able to change your type!"));
        message_window_ptr->exec();
        return;
    }

    // Выбранный тип.
    const int user_type = ui->cbUserType->currentIndex()+1;

    // Проверяем, не совпадает ли выбранный тип с уже установленным.
    if (user_type == data_keeper_ptr->get_user_data(user_id)->login_type.user_type) {
        message_window_ptr->set_message(QString("Old and new user types are the same"));
        message_window_ptr->exec();
        return;
    }

    lock_buttons();

    // Отправляем запрос на изменение типа пользователя.
    if ( !request_manager_ptr->send_set_usertype(user_id, user_type) ) {
        message_window_ptr->set_message(request_manager_ptr->get_last_error());
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Set)) {
        message_window_ptr->set_message(QString("Unable to change type of the choosen user\n%1").arg(error_text));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Запоминаем новый тип пользователя.
    data_keeper_ptr->set_user_type(user_id, user_type);

    // Обновляем содержимое ячейки в таблице.
    users_table_model->setData(users_table_model->index(selection.at(0).row(), 2), collector_ptr->type_description(user_type), Qt::DisplayRole);

    message_window_ptr->set_message(QString("User type has been successfully changed to %1")
                                        .arg(collector_ptr->type_description(user_type)));
    message_window_ptr->exec();
    unlock_buttons();
}

// Запрос на изменение пароля.
void AdminWindow::change_password()
{
    QModelIndexList selection = ui->tvUsers->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        message_window_ptr->set_message(QString("Choose a user to change password"));
        message_window_ptr->exec();
        return;
    }

    if (!index_y_user_id_map.contains(selection.at(0).row())) {
        message_window_ptr->set_message(QString("Unexpected index error!"));
        message_window_ptr->exec();
        return;
    }

    const int user_id = index_y_user_id_map[selection.at(0).row()];

    if (!data_keeper_ptr->users_containes(user_id)) {
        return;
    }

    // Запрашиваем новый пароль.
    passwd_window_ptr->clear_fields();
    if ( passwd_window_ptr->exec() == QDialog::Rejected ) {
        return;
    }

    const QString pass_value_1 = passwd_window_ptr->get_first_value();
    const QString pass_value_2 = passwd_window_ptr->get_second_value();

    // Не должно быть пустого значения пароля.
    if (pass_value_1.isEmpty() || pass_value_2.isEmpty()) {
        message_window_ptr->set_message(QString("Both password fields have to be filled in!"));
        message_window_ptr->exec();
        return;
    }

    // Оба значения должны совпадать.
    if (pass_value_1 != pass_value_2) {
        message_window_ptr->set_message(QString("Password values do not match!"));
        message_window_ptr->exec();
        return;
    }

    lock_buttons();

    // Отправляем запрос на изменение пароля пользователя.
    if ( !request_manager_ptr->send_set_password(user_id, pass_value_1) ) {
        message_window_ptr->set_message(request_manager_ptr->get_last_error());
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Set)) {
        message_window_ptr->set_message(QString("Unable to change password for the choosen user\n%1").arg(error_text));
        message_window_ptr->exec();
        unlock_buttons();
        return;
    }

    message_window_ptr->set_message(QString("Password for user %1 has been successfully changed")
                                        .arg(data_keeper_ptr->get_user_data(user_id)->login_type.user_name));
    message_window_ptr->exec();
    unlock_buttons();
}
