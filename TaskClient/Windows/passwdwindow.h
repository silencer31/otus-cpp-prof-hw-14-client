#ifndef PASSWDWINDOW_H
#define PASSWDWINDOW_H

#include <QDialog>

namespace Ui {
class PasswdWindow;
}

class PasswdWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PasswdWindow(QWidget *parent = nullptr);

    ~PasswdWindow();

    QString get_first_value();

    QString get_second_value();

    void clear_fields();

    void change_labels();

private:
    Ui::PasswdWindow *ui;
};

using passwd_win_shared = QSharedPointer<PasswdWindow>;

#endif // PASSWDWINDOW_H
