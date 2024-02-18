#include "passwdwindow.h"
#include "ui_passwdwindow.h"

PasswdWindow::PasswdWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PasswdWindow)
{
    ui->setupUi(this);

    // Убирает верхний хэдер окна.
    setWindowFlags(Qt::WindowTitleHint | Qt::ToolTip);

    connect(ui->pbOk,     SIGNAL(clicked(bool)),   this, SLOT(accept()) );
    connect(ui->pbCancel, SIGNAL(clicked(bool)),   this, SLOT(reject()) );
    connect(ui->leFirst,  SIGNAL(returnPressed()), this, SLOT(accept()) );
    connect(ui->leSecond, SIGNAL(returnPressed()), this, SLOT(accept()) );
}

QString PasswdWindow::get_first_value()
{
    return ui->leFirst->text();
}

QString PasswdWindow::get_second_value()
{
    return ui->leSecond->text();
}

void PasswdWindow::clear_fields()
{
    ui->leFirst->clear();
    ui->leSecond->clear();
}

void PasswdWindow::set_labels_server_port()
{
    ui->labelFirst->setText(QString("Server IP address"));
    ui->labelSecond->setText(QString("Server port"));
}

void PasswdWindow::set_labels_ask_login()
{
    ui->labelFirst->setText(QString("Login"));
    ui->labelSecond->setText(QString("Password"));
}

void PasswdWindow::set_labels_new_password()
{
    ui->labelFirst->setText(QString("New password"));
    ui->labelSecond->setText(QString("Repeat password"));
}

PasswdWindow::~PasswdWindow()
{
    delete ui;
}
