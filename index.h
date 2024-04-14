#ifndef INDEX_H
#define INDEX_H

#include <QWidget>
#include <QDesktopWidget>
//time and date
#include <ctime>
#include <QTimer>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <QDebug>

//text browsing
#include <QFile>
#include <QTextStream>
#include <QStringListModel>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class Index; }
QT_END_NAMESPACE

class Index : public QWidget
{
    Q_OBJECT

public:
    Index(QWidget *parent = nullptr);
    ~Index();


public Q_SLOT:   //function declarations
    //void updateTime();//slot to update time
    void UpdateSearchDrugOutcome(const QString &text);
    void navigatePages(int index);

private slots:
    void on_btn_exit_clicked();

    void on_btn_logout_clicked();

    void on_sale_btn_discard_clicked();

    void on_sale_btn_new_records_clicked();

    void on_btn_settings_clicked();

    void on_engine_On_clicked();

    void on_engine_Off_clicked();

    void on_browse_stock_photo_clicked();

    void on_browse_user_photo_clicked();

private:
    Ui::Index *ui;

    //text browsing class
    QStringList drugsData;

};
#endif // INDEX_H
