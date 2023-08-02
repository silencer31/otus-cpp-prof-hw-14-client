#include "Network/netcommunication.h"
#include "Network/requestmanager.h"

#include "Windows/messagewindow.h"
#include "Windows/adminwindow.h"
#include "Windows/operatorwindow.h"

#include "Windows/userwindow.h"
#include "Windows/loginrequest.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::string server_addr("127.0.0.1");
    int server_port = 2019;

    // Объект для сетевого взаимодействия.
    net_comm_shared net_communication_ptr = std::make_shared<NetCommunication>(server_addr, server_port);

    // Менеджер запросов к серверу.
    req_mngr_shared request_manager_ptr = std::make_shared<RequestManager>(net_communication_ptr);

    QSharedPointer<MessageWindow> message_window_ptr(new MessageWindow());  // Окно для показа сообщений.

    AdminWindow    admin_window(request_manager_ptr);    // Окно для задач администратора.
    admin_window.set_message_window(message_window_ptr);

    OperatorWindow operator_window(request_manager_ptr); // Окно для задач оператора базы данных
    operator_window.set_message_window(message_window_ptr);

    UserWindow     user_window(request_manager_ptr);     // Окно для задач обычного пользователя.
    user_window.set_message_window(message_window_ptr);

    LoginRequest   login_window;    // Окно для входа в систему.

    // Получение логина и пароля от пользователя.
    for(;;) {
        // Если пользователь нажмёт отмену при логине, завершаем программу.
        if ( login_window.exec() == QDialog::Rejected ) {
            return 0;
        }

        std::string login;
        std::string password;

        login_window.get_login_and_password(login, password);

        if ( request_manager_ptr->login_on_server(login, password) ) {
            break;
        }

        login_window.clear_fields();
    }

    return a.exec();
}
