#include "adminwindow.h"
#include "operatorwindow.h"
#include "userwindow.h"
#include "loginrequest.h"
#include "messagewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSharedPointer<MessageWindow> message_window_ptr(new MessageWindow());  // Окно для показа сообщений.

    AdminWindow    admin_window;    // Окно для задач администратора.
    admin_window.set_message_window(message_window_ptr);

    OperatorWindow operator_window; // Окно для задач оператора базы данных
    operator_window.set_message_window(message_window_ptr);

    UserWindow     user_window;     // Окно для задач обычного пользователя.
    user_window.set_message_window(message_window_ptr);

    LoginRequest   login_window;    // Окно для входа в систему.

            //w.show();
    return a.exec();
}
