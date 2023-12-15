#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

#include "messagewindow.h"
#include "Network/requestmanager.h"
#include "Parser/replyparser.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    LoginWindow() = delete;

    explicit LoginWindow(const req_mngr_shared rm_ptr, const parser_shared par_ptr,
                         const message_win_shared mw_ptr, QWidget *parent = nullptr);

    ~LoginWindow();

    const QString& get_user_name() {
        return user_name;
    }

    void clear_fields();

private slots:
    void handle_login();

private:
    Ui::LoginWindow *ui;

    const req_mngr_shared request_manager_ptr;

    const parser_shared parser_ptr; // Парсер json ответов от сервера.

    const message_win_shared message_window_ptr;


    QString user_name;
};

using login_win_unique = QScopedPointer<LoginWindow>;

#endif // LOGINWINDOW_H
