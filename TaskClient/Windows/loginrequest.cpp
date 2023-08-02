#include "loginrequest.h"
#include "ui_loginrequest.h"

LoginRequest::LoginRequest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginRequest)
{
    ui->setupUi(this);

    // Убирает верхний хэдер окна.
    setWindowFlags(Qt::WindowTitleHint | Qt::ToolTip);

    connect(ui->pbConfirm, SIGNAL(clicked(bool)), this, SLOT(accept()) );
    connect(ui->pbCancel,  SIGNAL(clicked(bool)), this, SLOT(reject()) );
}

void LoginRequest::get_login_and_password(std::string& login, std::string& password)
{
    login = ui->loginLineEdit->text().toStdString();
    password = ui->passwordLineEdit->text().toStdString();
}

void LoginRequest::clear_fields()
{
    ui->loginLineEdit->clear();
    ui->passwordLineEdit->clear();
}

LoginRequest::~LoginRequest()
{
    delete ui;
}
