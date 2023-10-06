#include "userwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(const req_mngr_shared rm_ptr, const message_win_shared mw_ptr, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
    , request_manager_ptr(rm_ptr)
    , message_window_ptr(mw_ptr)
{
    ui->setupUi(this);

    connect(ui->pbExit, SIGNAL(clicked(bool)), this, SLOT(quit()) );
}

UserWindow::~UserWindow()
{
    delete ui;
}
