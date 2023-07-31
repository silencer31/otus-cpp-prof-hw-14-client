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

    void get_login_and_password(std::string& login, std::string& password);

    void clear_fields();

private:
    Ui::LoginRequest *ui;
};

#endif // LOGINREQUEST_H
