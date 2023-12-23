#include "adminwindow.h"
#include "ui_adminwindow.h"

AdminWindow::AdminWindow(const req_mngr_shared rm_ptr, const collector_shared cltr_ptr, const parser_shared par_ptr,
                         const message_win_shared mw_ptr, const QString& uname, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow)
    , request_manager_ptr(rm_ptr)
    , collector_ptr(cltr_ptr)
    , parser_ptr(par_ptr)
    , message_window_ptr(mw_ptr)
    , user_name(uname)
    , user_id(collector_ptr->get_user_id())
{
    ui->setupUi(this);

    connect(ui->pbShutdown, SIGNAL(clicked(bool)), this, SLOT(shutdown()) );
    connect(ui->pbExit, SIGNAL(clicked(bool)), this, SLOT(close()) );

    ui->leUserId->setText(QString::number(user_id));
    ui->leUserName->setText(user_name);
}

void AdminWindow::check_server()
{
    if (!request_manager_ptr->connected_to_server()) {
        message_window_ptr->set_message(QString("No connection with server!"));
        message_window_ptr->exec();
        return;
    }

    // Отправляем тестовый запрос, чтобы узнать, готов ли сервер обрабатывать запросы.
    if (!request_manager_ptr->send_test_request()) {
        message_window_ptr->set_message(QString("Unable to send test request!\n%1")
                                        .arg(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Test)) {
        message_window_ptr->set_message(QString("Server is unable to handle requests!\n%1").arg(error_text));
        message_window_ptr->exec();
    }

    message_window_ptr->set_message(QString("Server shutdown completed.\nFurther communication impossible"));
    message_window_ptr->exec();
}


void AdminWindow::shutdown_server()
{
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

    message_window_ptr->set_message(QString("Server shutdown completed.\nFurther communication impossible"));
    message_window_ptr->exec();
}

bool AdminWindow::handle_request(CommandType comm_type)
{
    // Пытаемся получить ответ от сервера.
    if (!request_manager_ptr->get_server_answer(server_reply)) {
        error_text = QString("Unable to read server reply!\n%1").arg(request_manager_ptr->get_last_error());
        return false;
    }

    // Пробуем распарсить json в ответе.
    if ( !parser_ptr->handle_reply(comm_type , server_reply)) {
       error_text = QString("Unable to parse server reply\n%1").arg(parser_ptr->get_last_error());
       return false;
    }

    // Проверка наличия ошибки в запросе
    if (collector_ptr->req_error()) {
        error_text = QString("Unable to perform request!\n%1").arg(collector_ptr->get_error_text());
        return false;
    }

    // Проверка результата выполнения запроса.
    if (!collector_ptr->get_result()) {
        error_text = collector_ptr->get_details();
        return false;
    }

    return true;
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

