#include "databasehandler.h"
#include <QBuffer>
#include <QImageWriter>

DatabaseHandler::DatabaseHandler(QObject *parent) : QObject(parent)
{

}

//distructor
DatabaseHandler::~DatabaseHandler()
{
    closeDatabase();
}

//setting up sever connection
bool DatabaseHandler::OpenDatabase(const QString &host, const QString &databaseNane, const QString &user, const QString &password)
{
    //database connection credentials
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setUserName(user);
    db.setPassword(password);
    db.setHostName(host);
    db.setPort(5432);
    db.setDatabaseName(databaseNane);

    if(db.open()) // check is server connection is open or not
    {
        qDebug() << "database connected";
        return true;

    }else
    {
        qDebug() << "database not connected";
        return false;
    }
}

//close database to release resources slot
void DatabaseHandler::closeDatabase()
{
    if(db.open())
    {
        db.close();
    }
}

//query slot to handle login authentication
bool DatabaseHandler::UserLogin(const QString &status, const QString &email, const QString &password)
{
    if(!db.open())
    {
        qDebug() << "database is not opened";
        return false;
    }

    //preparing to query colums from login table in the database, bind the login inputs to the placeholders
    QSqlQuery query;
    query.prepare("SELECT status, email, pass FROM public.ADD_USER WHERE status = ? AND email = ? AND pass = ?");
    query.bindValue(0, status);
    query.bindValue(1, email);
    query.bindValue(2, password);

    //check if the query executed successfully or not
    if(query.exec())
    {
        if(query.next())
        {
            return true;
        }else if(status.isEmpty() || email.isEmpty() || password.isEmpty())
        {
            QMessageBox::warning(nullptr,"Login Authentication", "Input fields should not be filled", QMessageBox::Ok);
            return false;
        } else
        {
            QMessageBox::warning(nullptr,"Login Authentication", "Invalid credentials", QMessageBox::Ok);
            return false;
        }

    }else
    {        qDebug()<<"login query failed";
        closeDatabase();
        return false;
    }
}

//Insert new users
bool DatabaseHandler::NewUsers(const QString &Id, const QString &name, const QString &status, const QString &phone, const QString &email, const QString &password, const QImage &photo, const QDate &date)
{
    try{
        if(!db.open())
        {
            qDebug() << "database is not opened"<<db.lastError();
            return false;
        }

        // Convert image to byte array
        QByteArray imageArray;
        QBuffer buffer(&imageArray);
        buffer.open(QIODevice::WriteOnly);
        if (!photo.isNull())
        {
            if (!photo.save(&buffer, "JPG"))
            {
                qDebug() << "Failed to convert image to byte array";
            }
        }

        //check for empty inputs
        if(Id.isEmpty() || name.isEmpty() || status.isEmpty() || phone.isEmpty() || email.isEmpty() || password.isEmpty())
        {
            qDebug()<<"Fill all forms";
            return false;
        }

        //preparing to query colums from login table in the database, bind the login inputs to the placeholders
        QSqlQuery query;
        query.prepare("INSERT INTO public.ADD_USER(UserId, Name, Status, Phone, Email, Pass, Photo, Issued_Date) VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
        query.bindValue(0, Id);
        query.bindValue(1, name);
        query.bindValue(2, status);
        query.bindValue(3, phone);
        query.bindValue(4, email);
        query.bindValue(5, password);
        query.bindValue(6, imageArray);
        query.bindValue(7, date);

        if(query.exec())
        {
            qDebug() << "new user insert successfully";
            closeDatabase();
            return true;

        }else
        {
            qDebug() << "new user failed to insert"<<query.lastError();
            closeDatabase();
            return false;
        }

        return true;
    }catch (const std::exception &e)
    {
        qDebug()<<"Exception caught: "<<e.what();
        return false;
    }
}

//Insert items to stock table
bool DatabaseHandler::myStock(const QString &batchCode, const QString &itemName, const QString &category, const QString &quantityStatus, const int &quantity, const float &costPrice, const float &Sellingprice, const QString &recommend, const QString &shell, const QString &supplier, const QString &phone, const QString &description, const QImage &photo, const QDate &mfgDte, const QDate &expireDate)
{
    if(!db.open())
    {
        qDebug()<<"database not opened"<<db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO public.STOCK() VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.bindValue(0, batchCode);
    query.bindValue(1, itemName);
    query.bindValue(2, category);
    query.bindValue(3, quantityStatus);
    query.bindValue(4, quantity);
    query.bindValue(5, costPrice);
    query.bindValue(6, Sellingprice);
    query.bindValue(7, recommend);
    query.bindValue(8, shell);
    query.bindValue(9, supplier);
    query.bindValue(10, phone);
    query.bindValue(11, description);
    query.bindValue(12, photo);
    query.bindValue(13, mfgDte);
    query.bindValue(14, expireDate);

    if(batchCode.isEmpty() || itemName.isEmpty() || category.isEmpty() || quantityStatus.isEmpty() || quantity == 0 || costPrice == 0.0 || Sellingprice == 0.0 || recommend.isEmpty() || shell.isEmpty() || phone.isEmpty())
    {
        qDebug()<<"some fields can't be blank"<<query.lastError();

        return false;
    }else if(query.exec())
    {
        qDebug()<<"stock record insert successfull";
        return true;
    }else
    {
        qDebug()<<"query execution failed"<<query.lastError();
        return false;
    }
}


//sales record from stock table
bool DatabaseHandler::sales(const QString &sales_iD, const QString &batchCode, const QString &itemName, const QString &category, const QString &quantityStatus, const int &quantity, const float &Sellingprice, const QString &recommend, const QString &shell, const QString &description, const int &remains, const QDate &expireDate, const float &tax, const float &discount, const float &grosstotal, const QString &method, const float &payAmount, const QString &accountNumber, const QString &paidNumber, const float &change, const float &Total, const QString &custName, const QString &custPhone, float &DailySales)
{
    if(!db.open())
    {
        qDebug()<<"database not opened"<<db.lastError().text();
        return false;
    }
    //save some records into sales, customer & payment tables
    QSqlQuery Salesquery;
    QSqlQuery payquery;

    Salesquery.prepare("INSERT INTO public.SAlES(SalesId, BatchCode, ItemName, Category, QuantityStatus, Quantity, Recommend, SellingPrice, Shell, Description, PaymentId, SaleDate, ExpireDate) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    Salesquery.bindValue(0, sales_iD);
    Salesquery.bindValue(1, batchCode);
    Salesquery.bindValue(2, itemName);
    Salesquery.bindValue(3, category);
    Salesquery.bindValue(4, quantityStatus);
    Salesquery.bindValue(5, quantity);
    Salesquery.bindValue(6, recommend);
    Salesquery.bindValue(7, Sellingprice);
    Salesquery.bindValue(8, shell);
    Salesquery.bindValue(9, description);
    Salesquery.bindValue(10, remains);
    Salesquery.bindValue(11, expireDate);

    payquery.prepare("INSERT INTO public.Payments(Tax, Discount, Gross, Methods, Cash, AccountNo, PaidNo, Change, Total, CustName ,Phone, DailySale) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    payquery.bindValue(0, tax);
    payquery.bindValue(1, discount);
    payquery.bindValue(2, grosstotal);
    payquery.bindValue(3, method);
    payquery.bindValue(4, payAmount);
    payquery.bindValue(5, accountNumber);
    payquery.bindValue(6, paidNumber);
    payquery.bindValue(7, change);
    payquery.bindValue(8, Total);
    payquery.bindValue(9, custName);
    payquery.bindValue(10, custPhone);
    payquery.bindValue(11, DailySales);

    if(batchCode.isEmpty() || itemName.isEmpty() || category.isEmpty() || quantityStatus.isEmpty() || quantity == 0 || payAmount == 0.0 || recommend.isEmpty() || shell.isEmpty())
    {
        qDebug()<<"All fields must be fields"<<Salesquery.lastError();

        return false;
    }else if(Salesquery.exec() && payquery.exec())
    {
        qDebug()<<"sales record insert successfully";
        return true;
    }else
    {
        qDebug()<<"query execution failed"<<Salesquery.lastError();
        return false;
    }
}
