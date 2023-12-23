#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(const req_mngr_shared rm_ptr, const parser_shared par_ptr,
                         const message_win_shared mw_ptr, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
    , request_manager_ptr(rm_ptr)
    , parser_ptr(par_ptr)
    , message_window_ptr(mw_ptr)
{
    ui->setupUi(this);

    // Убирает верхний хэдер окна.
    setWindowFlags(Qt::WindowTitleHint | Qt::ToolTip);

    connect(ui->pbConfirm, SIGNAL(clicked(bool)), this, SLOT(handle_login()) );
    connect(ui->pbCancel,  SIGNAL(clicked(bool)), this, SLOT(reject()) );
}

void LoginWindow::clear_fields()
{
    ui->loginLineEdit->clear();
    ui->passwordLineEdit->clear();
}

void LoginWindow::handle_login()
{
    user_name = ui->loginLineEdit->text();
    QString password  = ui->passwordLineEdit->text();

    if (user_name.isEmpty() || password.isEmpty()) {
        message_window_ptr->set_message(QString("Both fields must be filled in"));
        message_window_ptr->exec();
        return;
    }

    std::string server_reply;

    // Отправляем запрос логин на сервер.
    if ( !request_manager_ptr->send_login(user_name, password) ) {
        message_window_ptr->set_message(request_manager_ptr->get_last_error());
        message_window_ptr->exec();
        return;
    }

    // Пытаемся получить ответ от сервера.
    if (!request_manager_ptr->get_server_answer(server_reply)) {
        message_window_ptr->set_message(request_manager_ptr->get_last_error());
        message_window_ptr->exec();
        return;
    }

    // Пробуем распарсить в json ответ от сервера.
    if ( !parser_ptr->handle_reply( CommandType::Login, server_reply)) {
        message_window_ptr->set_message(QString("Unable to parse server reply\n%1").arg(parser_ptr->get_last_error()));
        message_window_ptr->exec();
        return;
    }

    this->accept();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}
