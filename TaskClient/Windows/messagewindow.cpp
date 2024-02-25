#include "messagewindow.h"
#include "ui_messagewindow.h"

MessageWindow::MessageWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageWindow)
{
    ui->setupUi(this);

    // Убирает верхний хэдер окна.
    setWindowFlags(Qt::WindowTitleHint | Qt::ToolTip);

    connect(ui->pbConfirm, SIGNAL(clicked(bool)), this, SLOT(accept()) );

    ui->pbConfirm->setFocus();
}

MessageWindow::~MessageWindow()
{
    delete ui;
}

void MessageWindow::set_message(const QString& message)
{
    ui->messageField->clear();

    foreach(QString str, message.split("\n")) {
        ui->messageField->append(str);
        ui->messageField->setAlignment(Qt::AlignCenter);
    }

    ui->pbConfirm->setFocus();
}
