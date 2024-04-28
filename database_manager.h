#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QWidget>
#include <QtSql/QSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>
#include <QMessageBox>

class DataBase_Manager : public QWidget
{
    Q_OBJECT
public:
    explicit DataBase_Manager(QWidget *parent = nullptr);

signals:

};

#endif // DATABASE_MANAGER_H
