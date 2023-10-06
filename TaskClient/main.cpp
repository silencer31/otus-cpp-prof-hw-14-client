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

    std::string server_addr("127.0.0.1");
    int server_port = 2019;

    // Объект для сетевого взаимодействия.
    const net_comm_shared net_communication_ptr = std::make_shared<NetCommunication>(server_addr, server_port);

    // Менеджер запросов к серверу.
    const req_mngr_shared request_manager_ptr = std::make_shared<RequestManager>(net_communication_ptr);

    // Отправляем тестовое сообщение на сервер.
    //request_manager_ptr->send_test();

    // Окно для показа сообщений.
    const message_win_shared message_window_ptr(new MessageWindow());

    // Окно для задач администратора.
    const admin_win_unique admin_window_ptr(new AdminWindow(request_manager_ptr, message_window_ptr));

    // Окно для задач оператора базы данных.
    const operator_win_unique operator_window_ptr(new OperatorWindow(request_manager_ptr, message_window_ptr));

    // Окно для задач обычного пользователя.
    const user_win_unique user_window_ptr(new UserWindow(request_manager_ptr, message_window_ptr));

    // Окно для входа в систему.
    LoginRequest   login_window;

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
