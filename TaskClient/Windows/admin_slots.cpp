#include "adminwindow.h"
#include "ui_adminwindow.h"

void AdminWindow::get_users_list()
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

    // Проверяем, был ли ранее получен список типов пользователей.
    if (collector_ptr->empty_user_types()) {
        // Запрос списка типа пользователей.
        if (!request_manager_ptr->send_get_typelist()) {
            message_window_ptr->set_message(QString("Unable to send get typelist request!\n\n%1")
                                            .arg(request_manager_ptr->get_last_error()));
            message_window_ptr->exec();
            unlock_buttons();
            return;
        }

        // Контроль выполнения запроса.
        if ( !handle_request(CommandType::Get)) {
            message_window_ptr->set_message(QString("Unable to get user types!\n\n%1").arg(error_text));
            message_window_ptr->exec();
            unlock_buttons();
            return;
        }
    }

    ui->textBrowser->clear();

    // Проверяем, что получено от сервера.
    if (collector_ptr->empty_user_types()) {
        ui->textBrowser->append(QString("Получена пустая коллекция типов пользователей"));
        unlock_buttons();
        return;
    }

    for(auto iter = collector_ptr->user_types_cib(); iter != collector_ptr->user_types_cie(); ++iter) {
        ui->textBrowser->append(*iter);
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
    if (collector_ptr->empty_user_ids()) {
        ui->textBrowser->append(QString("Получена пустая коллекция id пользователей"));
        unlock_buttons();
        return;
    }

    for(auto iter = collector_ptr->user_ids_cib(); iter != collector_ptr->user_ids_cie(); ++iter) {
        ui->textBrowser->append(QString::number(*iter));
    }

    unlock_buttons();
}

void AdminWindow::get_tasks_list()
{
    // Блокируем кнопки
    lock_buttons();

    if (collector_ptr->empty_user_types()) {
        if (request_manager_ptr->send_get_typelist()) {

        }
    }

    if (collector_ptr->empty_task_statuses()) {
        if (request_manager_ptr->send_get_statuslist()) {

        }
    }
/*
    request_manager_ptr->send_get_userlist();

    request_manager_ptr->send_get_tasklist();

    request_manager_ptr->send_get_taskdata();*/
}

void AdminWindow::apply_changes()
{

}

void AdminWindow::create_user()
{

}

void AdminWindow::set_new_password()
{

}

void AdminWindow::shutdown_server()
{
    // Есть ли связь с сервером.
    if (!request_manager_ptr->connected_to_server()) {
        message_window_ptr->set_message(QString("No connection with server!"));
        message_window_ptr->exec();
        return;
    }

    // Отправляем запрос на сервер.
    if ( !request_manager_ptr->send_shutdown()) {
        message_window_ptr->set_message(QString("Unable to send shutdown request!\n%1")
                                        .arg(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Shutdown)) {
        message_window_ptr->set_message(QString("Unable to shutdown server!\n%1").arg(error_text));
        message_window_ptr->exec();
    }

    request_manager_ptr->disconnect_from_server();

    message_window_ptr->set_message(QString("Server shutdown completed.\nFurther communication impossible"));
    message_window_ptr->exec();
}
