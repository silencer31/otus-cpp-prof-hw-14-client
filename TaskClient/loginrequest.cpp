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

LoginRequest::~LoginRequest()
{
    delete ui;
}
