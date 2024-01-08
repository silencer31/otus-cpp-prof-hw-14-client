#include "passwdwindow.h"
#include "ui_passwdwindow.h"

PasswdWindow::PasswdWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PasswdWindow)
{
    ui->setupUi(this);

    // Убирает верхний хэдер окна.
    setWindowFlags(Qt::WindowTitleHint | Qt::ToolTip);

    connect(ui->pbOk,     SIGNAL(clicked(bool)), this, SLOT(accept()) );
    connect(ui->pbCancel, SIGNAL(clicked(bool)), this, SLOT(reject()) );
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

void PasswdWindow::change_labels()
{
    ui->labelFirst->setText(QString("New password"));
    ui->labelSecond->setText(QString("Repeat password"));
}

PasswdWindow::~PasswdWindow()
{
    delete ui;
}
