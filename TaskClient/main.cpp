#include "Windows/adminwindow.h"
#include "Windows/operatorwindow.h"
#include "Windows/userwindow.h"

#include "Extra/extra_functions.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Окно для показа сообщений.
    const message_win_shared message_window_ptr(new MessageWindow());

    // Окно для запроса адреса и порта сервера, а далее для логина или смены пароля.
    const passwd_win_shared passwd_window_ptr(new PasswdWindow());

    int server_port{2019};
    std::string server_ip{"127.0.0.1"};

    // Зарашиваем ip адрес сервера и номер порта.
    for(;;) {
        // Запрашиваем адрес сервера и номер порта. Если пользователь нажмёт отмену, завершаем программу.
        if ( passwd_window_ptr->exec() == QDialog::Rejected ) {
            return 0;
        }

        const QString address_value = passwd_window_ptr->get_first_value();
        const QString port_value    = passwd_window_ptr->get_second_value();

        server_port = get_port(port_value);

        // Проверяем, что данные для связи с сервером введены корректно.
        if ((server_port < 0) || !check_address(address_value)) {
            message_window_ptr->set_message(QString("Connection parameters are not correct!\nIP value: %1\nPort value: %2")
                                            .arg(address_value, port_value));
            message_window_ptr->exec();
            return -1;
        }

        server_ip = address_value.toStdString();
    }

    // Менеджер запросов к серверу.
    const req_mngr_shared request_manager_ptr{ std::make_shared<RequestManager>(server_ip, server_port)};

    // Собиратель данных - ответов от сервера
    const collector_shared collector_ptr{ std::make_shared<Collector>()};

    // Парсер json ответов от сервера.
    const parser_shared parser_ptr{ std::make_shared<ReplyParser>(collector_ptr)};

    // Обработчик ответов от сервера на запросы.
    const handler_shared handler_ptr{ std::make_shared<Handler>(request_manager_ptr, collector_ptr, parser_ptr) };

    // Хранитель полученных данных о пользователях и задачах.
    const data_keeper_shared data_keeper_ptr{ std::make_shared<DataKeeper>()};

    // Проверка, удалось ли подключиться к серверу.
    if ( !request_manager_ptr->connected_to_server()) {
        message_window_ptr->set_message(QString("Unable to connect to server\n%1")
                                            .arg(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return -1;
    }

    // Запрос типов пользователей.
    if ( !handler_ptr->get_user_types()) {
        message_window_ptr->set_message(QString("Unable to get user types\n%1").arg(handler_ptr->get_error()));
        message_window_ptr->exec();
        return -1;
    }

    // Запрос возможных статусов задач.
    if ( !handler_ptr->get_task_statuses()) {
        message_window_ptr->set_message(QString("Unable to get task statuses\n%1").arg(handler_ptr->get_error()));
        message_window_ptr->exec();
        return -1;
    }


    /// Создаём главные окна программы.
    // Окно для задач администратора.
    const admin_win_unique admin_window_ptr(new AdminWindow(request_manager_ptr, collector_ptr, parser_ptr,
                                                            handler_ptr, data_keeper_ptr,
                                                            message_window_ptr, passwd_window_ptr));

    // Окно для задач оператора базы данных.
    const operator_win_unique operator_window_ptr(new OperatorWindow(request_manager_ptr, collector_ptr, parser_ptr,
                                                                     handler_ptr, data_keeper_ptr,
                                                                     message_window_ptr, passwd_window_ptr));

    // Окно для задач обычного пользователя.
    const user_win_unique user_window_ptr(new UserWindow(request_manager_ptr, collector_ptr, parser_ptr,
                                                         handler_ptr, data_keeper_ptr,
                                                         message_window_ptr, passwd_window_ptr));


    /// Пользователь пытается получить доступ и приступить к работе.
    // Данный цикл позволяет совершить логин на сервере и переключать работу под разными типами пользователей.
    for(;;) {
        // Устанавливаем надписи для возможности логина.
        passwd_window_ptr->set_labels_ask_login();

        // Если пользователь нажмёт отмену при логине, завершаем программу.
        if ( passwd_window_ptr->exec() == QDialog::Rejected ) {
            return 0;
        }

        const QString user_name = passwd_window_ptr->get_first_value();
        const QString password  = passwd_window_ptr->get_second_value();

        // Оба поля должны быть заполнены.
        if (user_name.isEmpty() || password.isEmpty()) {
            message_window_ptr->set_message(QString("Both fields must be filled in"));
            message_window_ptr->exec();
            continue;
        }

        passwd_window_ptr->clear_fields();

        // Пробуем выполнить логин на сервере.
        if ( !handler_ptr->login_on_server(user_name, password)) {
            // Показываем сообщение о неудачном логине.
            message_window_ptr->set_message(QString("Unable to login on server\n%1").arg(handler_ptr->get_error()));
            message_window_ptr->exec();
            continue;
        }

        // Т.к. логин был совершен удачно, меняем надписи для возможности смены пароля.
        passwd_window_ptr->set_labels_new_password();

        // Запоминаем актуальные данные текущего пользователя.
        data_keeper_ptr->set_own_login(user_name);
        data_keeper_ptr->set_own_id(collector_ptr->get_user_id_after_login());
        data_keeper_ptr->set_own_type(collector_ptr->get_user_type_after_login());

        UserType user_type{UserType::Unsupported};
        user_type = static_cast<UserType>(data_keeper_ptr->get_own_type());

        switch(user_type) {
        case UserType::Unsupported:
            message_window_ptr->set_message(QString("Unsupported user type: %1")
                                            .arg(QString::number(data_keeper_ptr->get_own_type())));
            message_window_ptr->exec();
            return 1;
        case UserType::Administrator:
            admin_window_ptr->output_user_data();
            admin_window_ptr->show();
            break;
        case UserType::Operator:
            operator_window_ptr->output_user_data();
            operator_window_ptr->show();
            break;
        case UserType::User:
            user_window_ptr->output_user_data();
            user_window_ptr->show();
            break;
        default:
            message_window_ptr->set_message(QString("Unsupported user type: %1")
                                            .arg(QString::number(data_keeper_ptr->get_own_type())));
            message_window_ptr->exec();
            return 1;
        }
    }

    return a.exec();
}
