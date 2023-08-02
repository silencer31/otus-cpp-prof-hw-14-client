#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H

#pragma once

#include <QDialog>

namespace Ui {
class MessageWindow;
}

class MessageWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MessageWindow(QWidget *parent = nullptr);

    ~MessageWindow();

    void set_message(const QString& message);

private:
    Ui::MessageWindow *ui;
};

#endif // MESSAGEWINDOW_H
