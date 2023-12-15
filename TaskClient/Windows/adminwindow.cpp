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

    connect(ui->pbExit, SIGNAL(clicked(bool)), this, SLOT(quit()) );
}

void AdminWindow::check_server()
{
    // Отправляем тестовый запрос, чтобы узнать, готов ли сервер обрабатывать запросы.
    if (!request_manager_ptr->send_test_request()) {
        message_window_ptr->set_message(QString::fromStdString(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return;
    }

    // Пытаемся получить ответ от сервера.
    if (!request_manager_ptr->get_server_answer(server_reply)) {
        message_window_ptr->set_message(QString::fromStdString(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return;
    }

    // Пробуем распарсить в json ответ от сервера.
    if ( !parser_ptr->handle_reply( CommandType::Test, server_reply)) {
        message_window_ptr->set_message(QString("Unable to parse server reply"));
        message_window_ptr->exec();
        return;
    }

    if (!collector_ptr->get_result()) {
        message_window_ptr->set_message(QString("Server is not ready"));
        message_window_ptr->exec();
        return;
    }
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

