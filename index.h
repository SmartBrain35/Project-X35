#ifndef INDEX_H
#define INDEX_H
#include <databasehandler.h>
#include <QWidget>
#include <QDesktopWidget>

//time and date
#include <ctime>
#include <QTime>
#include <iomanip>
#include <QDebug>

//text browsing
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

//sound engine
#include <QtTextToSpeech/QTextToSpeech>
#include <QVector>
#include <QLoggingCategory>

//general
#include <QStringListModel>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QString>

//tray pop-up
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>

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
    void reportPageNavigation(int index);
    void SettingUser();

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
    void HandleACtion();
    void trayMessage(const QString &title, const QString &message, int duration = 60);
    void updateTime();

private slots:
    void on_btn_exit_clicked();

    void on_btn_logout_clicked();

    void on_sale_btn_discard_clicked();

    void on_sale_btn_new_records_clicked();

    void on_engine_On_clicked();

    void on_engine_Off_clicked();

    void on_browse_stock_photo_clicked();

    void on_browse_user_photo_clicked();


    void on_btn_add_NewUser_clicked();

    void on_sale_btn_save_clicked();

private:
    Ui::Index *ui;
    //database server
    DatabaseHandler handler;

    //speech`
    QTextToSpeech *m_speech;
    QVector<QVoice> m_voices;

    //text browsing class
    QStringList drugsData;

    //system tray
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;

    //timer class
    QTimer *timer = new QTimer();
    QTimer *timer2 = new QTimer();
};
#endif // INDEX_H
