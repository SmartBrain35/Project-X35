#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "databasehandler.h"

#include <QTimer>
#include <QMessageBox>
#include <QRandomGenerator>

namespace Ui {class Login;}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_btn_exit_clicked();

    void on_btn_login01_clicked();

public Q_SLOT:
    void hideLoginWidget();
    void OpenHomePage();

private:
    Ui::Login *ui;
    DatabaseHandler handler;
};

#endif // LOGIN_H
