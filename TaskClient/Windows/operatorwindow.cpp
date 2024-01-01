#include "operatorwindow.h"
#include "ui_operatorwindow.h"

OperatorWindow::OperatorWindow(const req_mngr_shared rm_ptr, const collector_shared cltr_ptr, const parser_shared par_ptr,
                               const message_win_shared mw_ptr, const QString& uname, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OperatorWindow)
    , request_manager_ptr(rm_ptr)
    , collector_ptr(cltr_ptr)
    , parser_ptr(par_ptr)
    , message_window_ptr(mw_ptr)
    , user_name(uname)
    , user_id(collector_ptr->get_own_id())
{
    ui->setupUi(this);

    connect(ui->pbAppointUser, SIGNAL(clicked(bool)), this, SLOT(appoint_user()) );
    connect(ui->pbSetStatus, SIGNAL(clicked(bool)), this, SLOT(set_task_status()) );
    connect(ui->pbDeleteTask, SIGNAL(clicked(bool)), this, SLOT(delete_task()) );
    connect(ui->pbGetTasks, SIGNAL(clicked(bool)), this, SLOT(get_tasks_list()) );
    connect(ui->pbSetDeadline, SIGNAL(clicked(bool)), this, SLOT(set_deadline()) );
    connect(ui->pbNewTask, SIGNAL(clicked(bool)), this, SLOT(create_task()) );

    connect(ui->pbExit, SIGNAL(clicked(bool)), this, SLOT(close()) );
}

void OperatorWindow::lock_buttons()
{
    ui->pbAppointUser->setEnabled(false);
    ui->pbSetStatus->setEnabled(false);
    ui->pbDeleteTask->setEnabled(false);
    ui->pbGetTasks->setEnabled(false);
    ui->pbSetDeadline->setEnabled(false);
    ui->pbNewTask->setEnabled(false);
    ui->pbExit->setEnabled(false);
}

void OperatorWindow::unlock_buttons()
{
    ui->pbAppointUser->setEnabled(true);
    ui->pbSetStatus->setEnabled(true);
    ui->pbDeleteTask->setEnabled(true);
    ui->pbGetTasks->setEnabled(true);
    ui->pbSetDeadline->setEnabled(true);
    ui->pbNewTask->setEnabled(true);
    ui->pbExit->setEnabled(true);
}

bool OperatorWindow::handle_request(CommandType comm_type)
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

OperatorWindow::~OperatorWindow()
{
    delete ui;
}
