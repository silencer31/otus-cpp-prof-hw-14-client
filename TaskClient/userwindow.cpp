#include "userwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(const request_manager_ptr rm_ptr, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
    , req_man_ptr(rm_ptr)
{
    ui->setupUi(this);

    connect(ui->pbExit, SIGNAL(clicked(bool)), this, SLOT(quit()) );
}

UserWindow::~UserWindow()
{
    delete ui;
}
