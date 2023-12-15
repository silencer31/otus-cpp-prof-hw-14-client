#include "Windows/loginwindow.h"
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

    // Окно для показа сообщений.
    const message_win_shared message_window_ptr(new MessageWindow());

    // Окно для входа в систему.
    const login_win_unique login_window_ptr(new LoginWindow(request_manager_ptr, parser_ptr, message_window_ptr));

    // Проверка, удалось ли подключиться к серверу.
    if ( !request_manager_ptr->connected_to_server()) {
        message_window_ptr->set_message(QString("Unable to connect to server\n%1")
                                        .arg(QString::fromStdString(request_manager_ptr->get_last_error())));
        message_window_ptr->exec();
        return 1;
    }

    /// Логин на сервере.
    for(;;) {
        // Если пользователь нажмёт отмену при логине, завершаем программу.
        if ( login_window_ptr->exec() == QDialog::Rejected ) {
            return 0;
        }

        // Проверяем, удалось ли провести логин на сервере.
        if (collector_ptr->get_result()) {
            break;
        }

        // Показываем сообщение о неудачном логине.
        message_window_ptr->set_message(QString("Unable to login on server\n%1").arg(collector_ptr->get_details()));
        message_window_ptr->exec();

        login_window_ptr->clear_fields();
    }

    // Окно для задач администратора.
    const admin_win_unique admin_window_ptr(new AdminWindow(request_manager_ptr, collector_ptr, parser_ptr,
                                                            message_window_ptr, login_window_ptr->get_user_name()));


    // Окно для задач оператора базы данных.
    const operator_win_unique operator_window_ptr(new OperatorWindow(request_manager_ptr, collector_ptr, parser_ptr,
                                                                     message_window_ptr, login_window_ptr->get_user_name()));


    // Окно для задач обычного пользователя.
    const user_win_unique user_window_ptr(new UserWindow(request_manager_ptr, collector_ptr, parser_ptr,
                                                         message_window_ptr, login_window_ptr->get_user_name()));

    UserType user_type{UserType::Unsupported};
    user_type = static_cast<UserType>(collector_ptr->get_user_type());

    switch(user_type) {
    case UserType::Unsupported:
        message_window_ptr->set_message(QString("Unsupported user type: %1").arg(QString::number(collector_ptr->get_user_type())));
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
        message_window_ptr->set_message(QString("Unsupported user type: %1").arg(QString::number(collector_ptr->get_user_type())));
        message_window_ptr->exec();
        return 1;
    }

    return a.exec();
}
