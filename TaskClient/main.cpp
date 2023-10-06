#include "Network/netcommunication.h"
#include "Network/requestmanager.h"

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

    // Окно для показа сообщений.
    QSharedPointer<MessageWindow> message_window_ptr(new MessageWindow());

    std::string server_addr("127.0.0.1");
    int server_port = 2019;

    // Объект для сетевого взаимодействия.
    net_comm_shared net_communication_ptr = std::make_shared<NetCommunication>(server_addr, server_port);

    // Проверка, удалось ли подключиться к серверу.
    if (net_communication_ptr->is_connected()) {
        std::cout << "Client connected to server. IP: " << server_addr << " Port: " << server_port << std::endl;
    }
    else {
        std::cout << "Connection error: " << net_communication_ptr->get_last_error() << std::endl;
        message_window_ptr->set_message(QString::fromStdString(net_communication_ptr->get_last_error()));
        message_window_ptr->exec();
        return 1;
    }

    // Менеджер запросов к серверу.
    req_mngr_shared request_manager_ptr = std::make_shared<RequestManager>(net_communication_ptr);

    // Отправляем тестовое сообщение на сервер.
    request_manager_ptr->send_test();

    // Окно для задач администратора.
    AdminWindow    admin_window(request_manager_ptr);
    admin_window.set_message_window(message_window_ptr);

    // Окно для задач оператора базы данных.
    OperatorWindow operator_window(request_manager_ptr);
    operator_window.set_message_window(message_window_ptr);

    // Окно для задач обычного пользователя.
    UserWindow     user_window(request_manager_ptr);
    user_window.set_message_window(message_window_ptr);

    // Окно для входа в систему.
    LoginRequest   login_window;

    // Получение логина и пароля от пользователя.
    for(;;) {
        // Если пользователь нажмёт отмену при логине, завершаем программу.
        if ( login_window.exec() == QDialog::Rejected ) {
            return 0;
        }

        std::string login;
        std::string password;

        login_window.get_login_and_password(login, password);

        if (login.empty() || password.empty()) {
            continue;
        }

        if ( request_manager_ptr->login_on_server(login, password) ) {
            break;
        }

        login_window.clear_fields();
    }

    return a.exec();
}
