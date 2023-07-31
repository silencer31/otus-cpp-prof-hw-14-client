#include "operatorwindow.h"
#include "ui_operatorwindow.h"

OperatorWindow::OperatorWindow(const request_manager_ptr rm_ptr, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OperatorWindow)
    , req_man_ptr(rm_ptr)
{
    ui->setupUi(this);

    connect(ui->pbExit, SIGNAL(clicked(bool)), this, SLOT(quit()) );
}

OperatorWindow::~OperatorWindow()
{
    delete ui;
}
