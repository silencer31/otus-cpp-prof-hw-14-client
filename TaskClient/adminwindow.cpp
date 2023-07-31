#include "adminwindow.h"
#include "ui_adminwindow.h"

AdminWindow::AdminWindow(const request_manager_ptr rm_ptr, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow)
    , req_man_ptr(rm_ptr)
{
    ui->setupUi(this);

    connect(ui->pbExit, SIGNAL(clicked(bool)), this, SLOT(quit()) );
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

