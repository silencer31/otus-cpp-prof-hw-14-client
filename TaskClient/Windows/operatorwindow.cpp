#include "operatorwindow.h"
#include "ui_operatorwindow.h"

OperatorWindow::OperatorWindow(const req_mngr_shared rm_ptr, const message_win_shared mw_ptr, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OperatorWindow)
    , request_manager_ptr(rm_ptr)
    , message_window_ptr(mw_ptr)
{
    ui->setupUi(this);

    connect(ui->pbExit, SIGNAL(clicked(bool)), this, SLOT(quit()) );
}

OperatorWindow::~OperatorWindow()
{
    delete ui;
}
