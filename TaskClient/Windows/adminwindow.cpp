#include "adminwindow.h"
#include "ui_adminwindow.h"

AdminWindow::AdminWindow(const req_mngr_shared rm_ptr, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow)
    , request_manager_ptr(rm_ptr)
{
    ui->setupUi(this);

    connect(ui->pbExit, SIGNAL(clicked(bool)), this, SLOT(quit()) );
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

