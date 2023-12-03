#include "Network/requestmanager.h"
#include "Parser/replyparser.h"

#include "Windows/messagewindow.h"
#include "Windows/adminwindow.h"
#include "Windows/operatorwindow.h"

#include "Windows/userwindow.h"
#include "Windows/loginrequest.h"


#include <QApplication>


#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::string server_addr{"127.0.0.1"};
    int server_port = 2019;

    // Менеджер запросов к серверу.
    const req_mngr_shared request_manager_ptr { std::make_shared<RequestManager>(server_addr, server_port)};

    // Собиратель данных - ответов от сервера
    const collector_shared collector_ptr { std::make_shared<Collector>() };

    // Парсер json ответов от сервера.
    const parser_shared parser_ptr{ std::make_shared<ReplyParser>(collector_ptr)};

    // Окно для показа сообщений.
    const message_win_shared message_window_ptr(new MessageWindow());

    // Окно для задач администратора.
    const admin_win_unique admin_window_ptr(new AdminWindow(request_manager_ptr, message_window_ptr));

    // Окно для задач оператора базы данных.
    const operator_win_unique operator_window_ptr(new OperatorWindow(request_manager_ptr, message_window_ptr));

    // Окно для задач обычного пользователя.
    const user_win_unique user_window_ptr(new UserWindow(request_manager_ptr, message_window_ptr));

    // Окно для входа в систему.
    LoginRequest login_window;

    // Проверка, удалось ли подключиться к серверу.
    if ( !request_manager_ptr->connected_to_server()) {
        std::cout << "Connection error: " << request_manager_ptr->get_last_error() << std::endl;
        message_window_ptr->set_message(QString::fromStdString(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return 1;
    }

    std::cout << "Client connected to server. IP: " << server_addr << " Port: " << server_port << std::endl;

    // Отправляем тестовый запрос, чтобы узнать, готов ли сервер обрабатывать запросы.
    if (!request_manager_ptr->send_test_request()) {
        std::cout << "Error occured while sending test request: " << request_manager_ptr->get_last_error() << std::endl;
        message_window_ptr->set_message(QString::fromStdString(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return 1;
    }

    std::string server_reply;

    // Пытаемся получить ответ от сервера.
    if (!request_manager_ptr->get_server_answer(server_reply)) {
        std::cout << "Error occured while receving reply for test request: " << request_manager_ptr->get_last_error() << std::endl;
        message_window_ptr->set_message(QString::fromStdString(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return 1;
    }

    // Пробуем распарсить в json ответ от сервера.
    if ( !parser_ptr->handle_reply( CommandType::Test, server_reply)) {
        std::cout << "Unable to parse server reply" << std::endl;
        message_window_ptr->set_message(QString("Unable to parse server reply"));
        message_window_ptr->exec();
        return 1;
    }

    std::cout << "Test request: " << (collector_ptr->get_result() ? "OK" : "Server is not ready") << std::endl;

    if (!collector_ptr->get_result()) {
        message_window_ptr->set_message(QString("Server is not ready"));
        message_window_ptr->exec();
        return 1;
    }

    // Получение логина и пароля от пользователя.
    std::string login;
    std::string password;

    for(;;) {
        // Если пользователь нажмёт отмену при логине, завершаем программу.
        if ( login_window.exec() == QDialog::Rejected ) {
            request_manager_ptr->disconnect_from_server();
            return 0;
        }

        login_window.get_login_and_password(login, password);

        if (login.empty() || password.empty()) {
            continue;
        }

        login_window.clear_fields();

        break;
    }

    if ( !request_manager_ptr->send_login(login, password) ) {
        std::cout << "Error occured while sending login request: " << request_manager_ptr->get_last_error() << std::endl;
        message_window_ptr->set_message(QString::fromStdString(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return 1;
    }

    server_reply.clear();

    // Пытаемся получить ответ от сервера.
    if (!request_manager_ptr->get_server_answer(server_reply)) {
        std::cout << "Error occured while receving reply for login request: : " << request_manager_ptr->get_last_error() << std::endl;
        message_window_ptr->set_message(QString::fromStdString(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return 1;
    }

    // Пробуем распарсить в json ответ от сервера.
    if ( !parser_ptr->handle_reply( CommandType::Login, server_reply)) {
        std::cout << "Unable to parse server reply" << std::endl;
        message_window_ptr->set_message(QString("Unable to parse server reply"));
        message_window_ptr->exec();
        return 1;
    }

    if (!collector_ptr->get_result()) {
        request_manager_ptr->disconnect_from_server();
        return 0;
    }

    UserType user_type{UserType::Unsupported};
    user_type = static_cast<UserType>(collector_ptr->get_user_type());

    switch(user_type) {
    case UserType::Unsupported:
        std::cout << "Unsupported user type: " << collector_ptr->get_user_type() << std::endl;
        message_window_ptr->set_message(QString("Unsupported user type").arg(QString::number(collector_ptr->get_user_type())));
        message_window_ptr->exec();
        return 1;
    case UserType::Administrator:
        admin_window_ptr->set_id(collector_ptr->get_user_id());
        admin_window_ptr->show();
        break;
    case UserType::Operator:
        operator_window_ptr->set_id(collector_ptr->get_user_id());
        operator_window_ptr->show();
        break;
    case UserType::User:
        user_window_ptr->set_id(collector_ptr->get_user_id());
        user_window_ptr->show();
        break;
    default:
        std::cout << "Unsupported user type: " << collector_ptr->get_user_type() << std::endl;
        message_window_ptr->set_message(QString("Unsupported user type").arg(QString::number(collector_ptr->get_user_type())));
        message_window_ptr->exec();
        return 1;
    }

    return a.exec();
}
