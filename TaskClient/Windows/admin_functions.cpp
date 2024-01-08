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

    // Отправляем запрос на создание нового пользователя.
    if ( !request_manager_ptr->send_add_user(user_name, (ui->cbUserType->currentIndex()+1), pass_value_1,
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


    message_window_ptr->set_message(QString("User %1 has been successfully created").arg(user_name));
    message_window_ptr->exec();
    unlock_buttons();
}

// Удалить выбранного пользователя.
void AdminWindow::delete_user()
{
    QModelIndexList selection = ui->tvUsers->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        message_window_ptr->set_message(QString("Choose a user before deletion!"));
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

    message_window_ptr->set_message(QString("User %1 has been successfully deleted!").arg(user_to_del));
    message_window_ptr->exec();
    unlock_buttons();
}

// Запрос на изменение данных пользователя.
void AdminWindow::change_user_data()
{

}

// Запрос на изменение пароля.
void AdminWindow::change_password()
{

}
