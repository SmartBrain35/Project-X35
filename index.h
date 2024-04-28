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

//sound engine
#include <QtTextToSpeech/QTextToSpeech>
#include <QVector>
#include <QLoggingCategory>

//database
#include <QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>

//general
#include <QMessageBox>
#include <QDebug>

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
    void graphicPageNavigation();
    //sound slots
    void speak();
    void stop();

    void setRate(int);
    void setPitch(int);
    void setVolume(int volume);

    void stateChanged(QTextToSpeech::State state);
    void engineSelected(int index);
    void languageSelected(int language);
    void voiceSelected(int index);

    void localeChanged(const QLocale &locale);
    void ReadLogMessage(QString log);

private slots:
    void on_btn_exit_clicked();

    void on_btn_logout_clicked();

    void on_sale_btn_discard_clicked();

    void on_sale_btn_new_records_clicked();

    void on_engine_On_clicked();

    void on_engine_Off_clicked();

    void on_browse_stock_photo_clicked();

    void on_browse_user_photo_clicked();

    void on_btn_connect_clicked();

private:
    Ui::Index *ui;
    QTextToSpeech *m_speech;
    QVector<QVoice> m_voices;

    //text browsing class
    QStringList drugsData;

};
#endif // INDEX_H
