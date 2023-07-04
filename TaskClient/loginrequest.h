#ifndef LOGINREQUEST_H
#define LOGINREQUEST_H

#include <QDialog>

namespace Ui {
class LoginRequest;
}

class LoginRequest : public QDialog
{
    Q_OBJECT

public:
    explicit LoginRequest(QWidget *parent = nullptr);
    ~LoginRequest();

private:
    Ui::LoginRequest *ui;
};

#endif // LOGINREQUEST_H
