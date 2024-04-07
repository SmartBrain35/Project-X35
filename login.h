#ifndef LOGIN_H
#define LOGIN_H
#include <QWidget>
#include <QTimer>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_btn_exit_clicked();

    void on_btn_login01_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
