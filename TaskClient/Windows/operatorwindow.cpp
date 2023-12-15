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
    , user_id(collector_ptr->get_user_id())
{
    ui->setupUi(this);

    connect(ui->pbExit, SIGNAL(clicked(bool)), this, SLOT(quit()) );
}

OperatorWindow::~OperatorWindow()
{
    delete ui;
}
