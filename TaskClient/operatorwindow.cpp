#include "operatorwindow.h"
#include "ui_operatorwindow.h"

OperatorWindow::OperatorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OperatorWindow)
{
    ui->setupUi(this);
}

OperatorWindow::~OperatorWindow()
{
    delete ui;
}
