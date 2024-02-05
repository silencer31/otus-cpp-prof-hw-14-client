#include "adminwindow.h"
#include "ui_adminwindow.h"

// Запрос на создание нового пользователя.
void AdminWindow::create_user()
{
    const QString user_name  = ui->leLogin->text();
    const QString surename   = ui->leSurename->text();
    const QString name       = ui->leName->text();
    const QString patronymic = ui->lePatronymic->text();

    // Проверка, заполнены ли поля.
    if (user_name.isEmpty() || surename.isEmpty() || name.isEmpty() || patronymic.isEmpty()) {
        show_message(QString("All the fields have to be filled in!"));
        return;
    }

    // Проверка, используется ли уже такой логин.
    if (data_keeper_ptr->user_name_received(user_name)) {
        show_message(QString("Login %1 is already in use!\nPlease enter another value").arg(user_name));
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
        show_message(QString("Both password fields have to be filled in!"));
        return;
    }

    // Оба значения должны совпадать.
    if (pass_value_1 != pass_value_2) {
        show_message(QString("Password values do not match!"));
        return;
    }

    const int user_type = ui->cbUserType->currentIndex()+1;

    lock_buttons();

    // Отправляем запрос на создание нового пользователя.
    if ( !handler_ptr->create_user(user_name, user_type, pass_value_1, surename, name, patronymic) ) {
        show_message( QString("Unable to create new user\n%1").arg(handler_ptr->get_error()));
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

    show_message(QString("User %1 has been successfully created").arg(user_name));

    unlock_buttons();
}

// Удалить выбранного пользователя.
void AdminWindow::delete_user()
{
    const QModelIndexList selection = ui->tvUsers->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        show_message(QString("Choose a user to delete"));
        return;
    }

    bool ok = false;
    const int user_id = users_table_model->item(selection.at(0).row())->data(Qt::DisplayRole).toInt(&ok);

    if (!ok || user_id <= 0) {
        return;
    }

    // Проверяем, нет ли попытки удалить себя.
    if (user_id == own_id) {
        show_message(QString("You are not able to delete yourself!"));
        return;
    }

    const QString user_to_del = data_keeper_ptr->get_user_data(user_id)->login_type.user_name;

    lock_buttons();

    // При удалении пользователя, все задачи, которые были на него назначены должны перейти в статус "not appointed".

    // Отправляем запрос на удаление пользователя.
    if ( !handler_ptr->delete_user(user_id) ) {
        show_message( QString("Unable to delete user %1\n%2").arg(user_to_del, handler_ptr->get_error()));
        unlock_buttons();
        return;
    }

    // Убираем пользователя из таблицы.
    users_table_model->removeRows(selection.at(0).row(), 1);
    ui->tvUsers->clearSelection();

    // Удаляем данные пользователя.
    data_keeper_ptr->del_user_data(user_id);

    // Изменяем отображение в таблице.
    for(int i = 0; i < tasks_table_model->rowCount(); ++i) {
        if (tasks_table_model->item(i, 5)->data(Qt::DisplayRole).toInt(&ok) != user_id) {
            continue;
        }

        tasks_table_model->setData(tasks_table_model->index(i, 1), collector_ptr->status_description(1), Qt::DisplayRole);
        tasks_table_model->setData(tasks_table_model->index(i, 5), QString("0"), Qt::DisplayRole);
        tasks_table_model->setData(tasks_table_model->index(i, 6), QString("No user"), Qt::DisplayRole);
    }

    // Всем задачам, на которые был назначен пользователь ставим статус "Not appointed" и user_id = 0.
    data_keeper_ptr->reset_tasks_status(user_id);

    show_message(QString("User %1 has been successfully deleted").arg(user_to_del));

    unlock_buttons();
}

// Запрос на изменение данных пользователя.
void AdminWindow::change_user_type()
{
    const QModelIndexList selection = ui->tvUsers->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        show_message(QString("Choose a user to change user type"));
        return;
    }

    bool ok = false;
    const int user_id = users_table_model->item(selection.at(0).row())->data(Qt::DisplayRole).toInt(&ok);

    if (!ok || user_id <= 0) {
        return;
    }

    if (!data_keeper_ptr->users_containes(user_id)) {
        return;
    }

    // Проверяем, нет ли попытки изменить свой тип.
    if (user_id == own_id) {
        show_message(QString("You are not able to change your type!"));
        return;
    }

    // Выбранный тип.
    const int user_type = ui->cbUserType->currentIndex()+1;

    // Проверяем, не совпадает ли выбранный тип с уже установленным.
    if (user_type == data_keeper_ptr->get_user_data(user_id)->login_type.user_type) {
        show_message(QString("Old and new user types are the same"));
        return;
    }

    lock_buttons();

    // Отправляем запрос на изменение типа пользователя.
    if ( !request_manager_ptr->send_set_usertype(user_id, user_type) ) {
        show_message(request_manager_ptr->get_last_error());
        unlock_buttons();
        return;
    }

    // Контроль выполнения запроса.
    if ( !handler_ptr->handle_request(CommandType::Set)) {
        show_message(QString("Unable to change type of the choosen user\n%1").arg(error_text));
        unlock_buttons();
        return;
    }

    // Запоминаем новый тип пользователя.
    data_keeper_ptr->set_user_type(user_id, user_type);

    // Обновляем содержимое ячейки в таблице.
    users_table_model->setData(users_table_model->index(selection.at(0).row(), 2), collector_ptr->type_description(user_type), Qt::DisplayRole);

    show_message(QString("User type has been successfully changed to %1")
                                .arg(collector_ptr->type_description(user_type)));

    unlock_buttons();
}

// Запрос на изменение пароля.
void AdminWindow::change_password()
{
    const QModelIndexList selection = ui->tvUsers->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        show_message(QString("Choose a user to change password"));
        return;
    }

    bool ok = false;
    const int user_id = users_table_model->item(selection.at(0).row())->data(Qt::DisplayRole).toInt(&ok);

    if (!ok || user_id <= 0) {
        return;
    }

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
        show_message(QString("Both password fields have to be filled in!"));
        return;
    }

    // Оба значения должны совпадать.
    if (pass_value_1 != pass_value_2) {
        show_message(QString("Password values do not match!"));
        return;
    }

    lock_buttons();

    // Отправляем запрос на изменение пароля пользователя.
    if ( !request_manager_ptr->send_set_password(user_id, pass_value_1) ) {
        show_message(request_manager_ptr->get_last_error());
        unlock_buttons();
        return;
    }

    // Контроль выполнения запроса.
    if ( !handler_ptr->handle_request(CommandType::Set)) {
        show_message(QString("Unable to change password for the choosen user\n%1").arg(error_text));
        unlock_buttons();
        return;
    }

    show_message(QString("Password for user %1 has been successfully changed")
                                .arg(data_keeper_ptr->get_user_data(user_id)->login_type.user_name));

    unlock_buttons();
}
