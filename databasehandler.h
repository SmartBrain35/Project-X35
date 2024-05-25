#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>
#include <QtDebug>
#include <QMessageBox>

class DatabaseHandler : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseHandler(QObject *parent = nullptr);
    ~DatabaseHandler();

    bool OpenDatabase(const QString &host, const QString &databaseNane, const QString &user, const QString &password);

    void closeDatabase();

    bool UserLogin(const QString &status, const QString &email, const QString &password);

    bool NewUsers(const QString &Id, const QString &name, const QString &status, const QString &phone, const QString &email, const QString &password, const QImage &photo, const QDate &date);

    bool myStock(const QString &batchCode, const QString &itemName, const QString &category, const QString &quantityStatus, const int &quantity, const float &costPrice, const float &Sellingprice, const QString &recommend, const QString &shell, const QString &supplier, const QString &phone, const QString &description, const QImage &photo, const QDate &mfgDte, const QDate &expireDate);

    bool sales(const QString &sales_iD, const QString &batchCode, const QString &itemName, const QString &category, const QString &quantityStatus, const int &quantity, const float &Sellingprice, const QString &recommend, const QString &shell, const QString &description, const int &remains, const QDate &expireDate, const float &tax, const float &discount, const float &grosstotal, const QString &method, const float &payAmount, const QString &accountNumber, const QString &paidNumber, const float &change, const float &Total, const QString &custName, const QString &custPhone, float &DailySales);

private:
    QSqlDatabase db;

};

#endif // DATABASEHANDLER_H
