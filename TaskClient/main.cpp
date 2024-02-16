#include "Windows/adminwindow.h"
#include "Windows/operatorwindow.h"
#include "Windows/userwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::string server_addr{"127.0.0.1"};
    int server_port = 2019;

    // Менеджер запросов к серверу.
    const req_mngr_shared request_manager_ptr{ std::make_shared<RequestManager>(server_addr, server_port)};

    // Собиратель данных - ответов от сервера
    const collector_shared collector_ptr{ std::make_shared<Collector>()};

    // Парсер json ответов от сервера.
    const parser_shared parser_ptr{ std::make_shared<ReplyParser>(collector_ptr)};

    // Обработчик ответов от сервера на запросы.
    const handler_shared handler_ptr{ std::make_shared<Handler>(request_manager_ptr, collector_ptr, parser_ptr) };

    // Хранитель полученных данных о пользователях и задачах.
    const data_keeper_shared data_keeper_ptr{ std::make_shared<DataKeeper>()};

    // Окно для показа сообщений.
    const message_win_shared message_window_ptr(new MessageWindow());

    // Окно для входа в систему.
    const passwd_win_shared passwd_window_ptr(new PasswdWindow());

    // Проверка, удалось ли подключиться к серверу.
    if ( !request_manager_ptr->connected_to_server()) {
        message_window_ptr->set_message(QString("Unable to connect to server\n%1")
                                        .arg(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return 1;
    }

    /// Логин на сервере.
    QString user_name;
    QString password;

    for(;;) {
        // Если пользователь нажмёт отмену при логине, завершаем программу.
        if ( passwd_window_ptr->exec() == QDialog::Rejected ) {
            return 0;
        }

        user_name = passwd_window_ptr->get_first_value();
        password  = passwd_window_ptr->get_second_value();

        // Оба поля должны быть заполнены.
        if (user_name.isEmpty() || password.isEmpty()) {
            message_window_ptr->set_message(QString("Both fields must be filled in"));
            message_window_ptr->exec();
            continue;
        }

        passwd_window_ptr->clear_fields();

        // Пробуем выполнить логин на сервере.
        if (handler_ptr->login_on_server(user_name, password)) {
            break;
        }

        // Показываем сообщение о неудачном логине.
        message_window_ptr->set_message(QString("Unable to login on server\n%1").arg(handler_ptr->get_error()));
        message_window_ptr->exec();
    }

    // Запрос типов пользователей.
    if ( !handler_ptr->get_user_types()) {
        message_window_ptr->set_message(QString("Unable to get user types\n%1").arg(handler_ptr->get_error()));
        message_window_ptr->exec();
        return 1;
    }

    // Запрос возможных статусов задач.
    if ( !handler_ptr->get_task_statuses()) {
        message_window_ptr->set_message(QString("Unable to get task statuses\n%1").arg(handler_ptr->get_error()));
        message_window_ptr->exec();
        return 1;
    }

    // В окне для логина и пароля меняем надписи для дальнейшего использования окна для смены пароля.
    passwd_window_ptr->change_labels();

    // Окно для задач администратора.
    const admin_win_unique admin_window_ptr(new AdminWindow(request_manager_ptr, collector_ptr, parser_ptr,
                                                            handler_ptr, data_keeper_ptr,
                                                            message_window_ptr, passwd_window_ptr, user_name));

    // Окно для задач оператора базы данных.
    const operator_win_unique operator_window_ptr(new OperatorWindow(request_manager_ptr, collector_ptr, parser_ptr,
                                                                     handler_ptr, data_keeper_ptr,
                                                                     message_window_ptr, passwd_window_ptr, user_name));

    // Окно для задач обычного пользователя.
    const user_win_unique user_window_ptr(new UserWindow(request_manager_ptr, collector_ptr, parser_ptr,
                                                         handler_ptr, data_keeper_ptr,
                                                         message_window_ptr, passwd_window_ptr, user_name));

    UserType user_type{UserType::Unsupported};
    user_type = static_cast<UserType>(collector_ptr->get_own_type());

    switch(user_type) {
    case UserType::Unsupported:
        message_window_ptr->set_message(QString("Unsupported user type: %1")
                                        .arg(QString::number(collector_ptr->get_own_type())));
        message_window_ptr->exec();
        return 1;
    case UserType::Administrator:
        admin_window_ptr->show();
        break;
    case UserType::Operator:
        operator_window_ptr->show();
        break;
    case UserType::User:
        user_window_ptr->show();
        break;
    default:
        message_window_ptr->set_message(QString("Unsupported user type: %1")
                                        .arg(QString::number(collector_ptr->get_own_type())));
        message_window_ptr->exec();
        return 1;
    }

    return a.exec();
}
