#include "index.h"
#include "ui_index.h"
#include "login.h"
#include <QBuffer>

Index::Index(QWidget *parent): QWidget(parent), ui(new Ui::Index), m_speech(0)
{
    ui->setupUi(this);



    //detecting screen size and fill the form wholy with it
    setContentsMargins(-1, 0, 0, 0);
    QDesktopWidget desktop;
    QRect screen = desktop.availableGeometry(this);
    setFixedSize(screen.width(), screen.height());

    //making the UI framless i.e without "title bar"
    // setWindowFlags(Qt::FramelessWindowHint);

    ui->stackedWidget->setHidden(true);//start with empty homepage

    //disable homepage buttons on app run
    ui->btn_sales->setDisabled(true);
    ui->btn_Stock->setDisabled(true);
    ui->btn_setting->setDisabled(true);
    ui->btn_statistics->setDisabled(true);

    //starting codes to open and load drug name from drug_list.txt to ui
    QFile file(":/new/Drugs/drugs_List.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QString line = in.readLine();
            drugsData.append(line);
        }
        file.close();
    }
    connect(ui->sale_drug_name, &QLineEdit::textChanged, this, &Index::UpdateSearchDrugOutcome);

    //****************navigation buttons to switch the pages using lamda expression*********************
    QObject::connect(ui->btn_sales, &QPushButton::clicked, this, [=](){navigatePages(0);});
    QObject::connect(ui->btn_setting, &QPushButton::clicked, this, [=](){navigatePages(1);});
    QObject::connect(ui->btn_Stock, &QPushButton::clicked, this, [=](){navigatePages(2);});
    QObject::connect(ui->btn_statistics, &QPushButton::clicked, this, [=](){navigatePages(3);});

    //connect buttons with the slot function to switch between the daily sales statistics form
    QObject::connect(ui->btn_next, &QPushButton::clicked, this, &Index::graphicPageNavigation);
    QObject::connect(ui->btn_back, &QPushButton::clicked, this, &Index::graphicPageNavigation);

    //****connect buttons with the slot function to switch between report forms
    QObject::connect(ui->btn_previous, &QPushButton::clicked, this, [=](){reportPageNavigation(0);});
    QObject::connect(ui->btn_forward, &QPushButton::clicked, this, [=](){reportPageNavigation(1);});

    //****setting up and call sound implementaion
    QLoggingCategory::setFilterRules(QStringLiteral("qt.speech.tts=true \n qt.speech.tts.*=true"));
    ui->plainTextEdit->setPlainText("hello world, welcome to smart pharmacy");

    // Populate engine selection list
    ui->engine->addItem("Default", QString("default"));
    foreach(QString engine, QTextToSpeech::availableEngines())
    {
        ui->engine->addItem(engine, engine);
    }
    ui->engine->setCurrentIndex(0);
    engineSelected(0);

    connect(ui->speakButton, &QPushButton::clicked, this, &Index::speak);
    connect(ui->pitch_slider, &QSlider::valueChanged, this, &Index::setPitch);
    connect(ui->rate_slider, &QSlider::valueChanged, this, &Index::setRate);
    connect(ui->volume_slider, &QSlider::valueChanged, this, &Index::setVolume);
    connect(ui->engine, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &Index::engineSelected);

    //system tray implementation
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/new/Icons/Megaphone.png"));

    trayMenu = new QMenu();
    trayMenu->addAction("Action activated");
    connect(trayMenu->actions().at(0), &QAction::triggered, this, &Index::HandleACtion);
    trayIcon->setContextMenu(trayMenu);

    //current animating time on the status bar slot called
    timer2 = new QTimer(this);
    connect(timer2, &QTimer::timeout, this, &Index::updateTime);
    timer2->start();
}

Index::~Index()
{
    delete ui;

}

//text browsing from external file from sale page while typing drug name
void Index::UpdateSearchDrugOutcome(const QString &text)
{
    QStringList My_Drugs;

    QStringList::ConstIterator be = drugsData.constBegin();
    QStringList::ConstIterator en = drugsData.constEnd();

    for(; be != en; ++be)
    {
        if(be->toLower().contains(text.toLower()))
        {
            My_Drugs.append(*be);
        }
    }
    ui->sale_drugs_list->setPlainText(My_Drugs.join("\n"));
}

//exit the application
void Index::on_btn_exit_clicked()
{
    qApp->quit();
}

//return to login page
void Index::on_btn_logout_clicked()
{
    static Login *login = nullptr;
    if(!login)
    {
        login = new Login();
    }
    close();
    login->show();
    login->hideLoginWidget();
    ui->stackedWidget->setHidden(true);
    ui->engine_Off->setChecked(true);
    ui->btn_sales->setDisabled(true);
    ui->btn_Stock->setDisabled(true);
    ui->btn_setting->setDisabled(true);
    ui->btn_statistics->setDisabled(true);

    m_speech->stop();
}

//clear payment method inputs
void Index::on_sale_btn_discard_clicked()
{
    ui->sale_show_discount->clear();
    ui->sale_show_paid_cash->clear();
    ui->sale_show_paid_number->clear();
    ui->sale_show_accnt_No->clear();
}

//clear all input boxes and prepare them for next sales record insertion
void Index::on_sale_btn_new_records_clicked()
{
    ui->sale_txt_batchCode->clear();
    ui->sale_drug_name->clear();
    ui->sale_quantity_counter->clear();
    ui->txt_customer_name->clear();
    ui->txt_customer_phone->clear();
}

//radio_button to start software whiles it enable some widgets
void Index::on_engine_On_clicked()
{
    ui->stackedWidget->setHidden(false);
    ui->btn_sales->setDisabled(false);
    ui->btn_Stock->setDisabled(false);
    ui->btn_setting->setDisabled(false);
    ui->btn_statistics->setDisabled(false);
    ui->stackedWidget->setCurrentIndex(0); //open sales form first
    ui->my_sales_tabs->setCurrentIndex(0);
    ui->engine_On->setEnabled(false);

    //testing expiring date with read out message
    QDate curDate = QDate::currentDate();
    if(curDate == curDate)
    {
        trayIcon->show();
        ReadLogMessage("Ideal Milk item with id number SP11004 on shell 12J has been expired\n action need to be taken in stock");
    }
}

//radio_button to clear homepage whiles it disable some widgets
void Index::on_engine_Off_clicked()
{
    ui->stackedWidget->setHidden(true);
    ui->btn_sales->setDisabled(true);
    ui->btn_Stock->setDisabled(true);
    ui->btn_setting->setDisabled(true);
    ui->btn_statistics->setDisabled(true);
    ui->engine_On->setEnabled(true);
    m_speech->stop(); //stop voice
}

//browsing image file for item on stock UI
void Index::on_browse_stock_photo_clicked()
{
    QString image_path = QFileDialog::getOpenFileName(nullptr, "Select Image", "","All Files(*.*)");

    if(!image_path.isEmpty())
    {
        QImage image(image_path);

        if(!image.isNull())
        {
            ui->item_pic->setPixmap(QPixmap::fromImage(image));
        }else
        {
            QMessageBox::critical(this, "Browsing image", "Fail to select image");
        }
    }
}

//browse image from local stograge and insert it to the user photo box
void Index::on_browse_user_photo_clicked()
{
    QString image_path = QFileDialog::getOpenFileName(nullptr, "Select Image", "","All Files(*.*)");

    if(!image_path.isEmpty())
    {
        QImage image(image_path);

        if(!image.isNull())
        {
            ui->user_photo->setPixmap(QPixmap::fromImage(image));
        }else
        {
            QMessageBox::critical(this, "Browsing image", "Fail to select image");
        }
    }
}

//slot or funntion to navigate through UI forms
void Index::navigatePages(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

//slot function to switch between graphic view daily statistics form
void Index::graphicPageNavigation()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if(button)
    {
        int currentIndex = ui->visual_stackedWidget->currentIndex();
        int count = ui->visual_stackedWidget->count();

        if(button == ui->btn_next)
        {
            int nextPage = ((currentIndex + 1) % count);
            ui->visual_stackedWidget->setCurrentIndex(nextPage);
        }else if(button == ui->btn_back)
        {
            int previousPage = ((currentIndex - 1 + count) %count);
            ui->visual_stackedWidget->setCurrentIndex(previousPage);
        }
    }
}

//slot function to switch between graphic view on report form
void Index::reportPageNavigation(int index)
{
    ui->stackedWidget_2->setCurrentIndex(index);
}

//sound slots
void Index::speak()
{
    m_speech->say(ui->plainTextEdit->toPlainText());
}

void Index::stop()
{
    m_speech->stop();
}

void Index::setRate(int rate)
{
    m_speech->setRate(rate / 100.0);
}

void Index::setPitch(int pitch)
{
    m_speech->setPitch(pitch / 10.0);
}

void Index::setVolume(int volume)
{
    m_speech->setVolume(volume / 100.0);
}

void Index::stateChanged(QTextToSpeech::State state)
{
    if(state == QTextToSpeech::Speaking)
    {
        ui->statusbar->setText("Speech started...");
    } else if (state == QTextToSpeech::Ready)
    {
        ui->statusbar->setText("Speech stopped...");
    }else if(state == QTextToSpeech::Paused)
    {
    }else
    {
        ui->statusbar->setText("Speech error!");
    }

    ui->pauseButton->setEnabled(state == QTextToSpeech::Speaking);
    ui->resumeButton->setEnabled(state == QTextToSpeech::Paused);
}

void Index::engineSelected(int index)
{
    QString engineName = ui->engine->itemData(index).toString();
    delete m_speech;
    if (engineName == "default")
    {
        m_speech = new QTextToSpeech(this);
    }else
    {
        m_speech = new QTextToSpeech(engineName, this);
    }
    disconnect(ui->language, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &Index::languageSelected);
    ui->language->clear();

    // Populate the languages combobox before connecting its signal.
    QVector<QLocale> locales = m_speech->availableLocales();
    QLocale current = m_speech->locale();
    foreach (const QLocale &locale, locales)
    {
        QString name(QStringLiteral("%1 (%2)").arg(QLocale::languageToString(locale.language())).arg(QLocale::countryToString(locale.country())));
        QVariant localeVariant(locale);
        ui->language->addItem(name, localeVariant);
        if (locale.name() == current.name())
        {
            current = locale;
        }
    }
    setRate(ui->rate_slider->value());
    setPitch(ui->pitch_slider->value());
    setVolume(ui->volume_slider->value());

    connect(ui->pauseButton, &QPushButton::clicked, m_speech, &QTextToSpeech::pause);
    connect(ui->resumeButton, &QPushButton::clicked, m_speech, &QTextToSpeech::resume);

    connect(m_speech, &QTextToSpeech::stateChanged, this, &Index::stateChanged);
    connect(m_speech, &QTextToSpeech::localeChanged, this, &Index::localeChanged);

    connect(ui->language, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &Index::languageSelected);
    localeChanged(current);
}

void Index::languageSelected(int language)
{
    QLocale locale = ui->language->itemData(language).toLocale();
    m_speech->setLocale(locale);
}

void Index::voiceSelected(int index)
{
    m_speech->setVoice(m_voices.at(index));
}

void Index::localeChanged(const QLocale &locale)
{
    QVariant localeVariant(locale);
    ui->language->setCurrentIndex(ui->language->findData(localeVariant));

    disconnect(ui->voice, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &Index::voiceSelected);
    ui->voice->clear();

    m_voices = m_speech->availableVoices();
    QVoice currentVoice = m_speech->voice();

    foreach (const QVoice &voice, m_voices)
    {
        ui->voice->addItem(QStringLiteral("%1 - %2 - %3").arg(voice.name()).arg(QVoice::genderName(voice.gender())) .arg(QVoice::ageName(voice.age())));

        if (voice.name() == currentVoice.name())
        {
            ui->voice->setCurrentIndex(ui->voice->count() - 1);
        }
    }
    connect(ui->voice, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &Index::voiceSelected);
}

//tray message slot
void Index::trayMessage(const QString &title, const QString &message, int duration)
{
    trayIcon->showMessage(title, message);
    connect(timer, &QTimer::timeout, this, &Index::HandleACtion);
    timer->start(duration);
}

//read out expire alert message
void Index::ReadLogMessage(QString log)
{
    ui->volume_slider->setValue(80); //set volume to 80%
    ui->rate_slider->setValue(1); // increase rate of speaking
    ui->pitch_slider->setValue(5); // set pitch to normall
    ui->voice->setCurrentIndex(0); //select female voice
    ui->log_message->setPlainText(log);//the message or alert
    m_speech->say(ui->log_message->toPlainText());//voice out the alert
    trayMessage("Expiration Alert", ui->log_message->toPlainText(), 12000); //call tray message display
}

//action to be taken slot
void Index::HandleACtion()
{
    trayIcon->hide();
}

//current time slot
void Index::updateTime()
{
    QTime myTime = QTime::currentTime();
    QString timeString = myTime.toString("hh : mm : ss AP");
    ui->lbl_time->setText(timeString);
}

void Index::on_btn_add_NewUser_clicked()
{
    QString idcard = ui->settings_user_Id->text();
    QString name = ui->settings_user_name->text();
    QString status;
    if(ui->admin_radioButton->isChecked())
    {
        status = ui->admin_radioButton->text();
    }else if(ui->sales_person_radioButton->isChecked())
    {
        status = ui->sales_person_radioButton->text();
    }else if(ui->supervisor_radioButton->isChecked())
    {
        status = ui->supervisor_radioButton->text();
    }else
    {
        status = "not specified";
    }

    QString phone = ui->settings_user_phone->text();
    QString email = ui->settings_user_email->text();
    QString pass = ui->settings_user_password->text();
    QImage photo = ui->user_photo->pixmap()->toImage();
    QDate dDate = QDate::currentDate();

    //NewUser function is method responsible for the inserting query in the Databasehandler class
    handler.NewUsers(idcard, name, status, phone, email, pass, photo, dDate);
}

//sales inputs slot
void Index::on_sale_btn_save_clicked()
{
    //generating random invoice id
    QRandomGenerator rand;
    int random = rand.bounded(1000, 9999);
    QString randomString = QString::number(random);

    QString prefix = "xp-";
    prefix.append(randomString);
    ui->invoice_id_label->text() = prefix; //showing result via Qlabel on sales UI

    QString sales_iD = prefix;
    QString batchCode = ui->sale_txt_batchCode->text();
    QString itemName = ui->sale_drug_name->text();
    QString category = ui->sale_txt_category->currentText();
    QString quantityStatus = ui->sale_quantity_status->currentText();
    int quantity = ui->sale_quantity_counter->value();
    float Sellingprice = ui->sale_txt_price->text().toFloat();

    QString recommend;// radio button
    if(ui->sale_kids_radiaButton->isChecked())
    {
        recommend = ui->sale_kids_radiaButton->text();
    }else if(ui->sale_adult_radiaButton->isChecked())
    {
        recommend = ui->sale_adult_radiaButton->text();
    }else if(ui->sale_male_radiaButton->isChecked())
    {
        recommend = ui->sale_male_radiaButton->text();
    }else if(ui->sale_female__radiaButton->isChecked())
    {
        recommend = ui->sale_female__radiaButton->text();
    }else
    {
        recommend = "not specified";
    }

    QString shell = ui->txt_shell_Id->text();
    QString description = ui->sale_description_edit->toPlainText();
    int remains = ui->sale_lcd_remain->value();
    QDate expireDate = QDate::currentDate();
    float tax = ui->sale_lcd_tax->value();
    float discount = ui->sale_show_discount->text().toFloat();
    float grosstotal = ui->sale_lcd_total->value();

    QString method; //payment method radioButton
    if(ui->sale_Mtn_radioButton->isChecked())
    {
        method = ui->sale_Mtn_radioButton->text();
    }else if(ui->sale_vodafone_radioButton->isChecked())
    {
        method = ui->sale_vodafone_radioButton->text();
    }else if(ui->sale_creditCard_radioButton->isChecked())
    {
        method = ui->sale_creditCard_radioButton->text();
    }else if(ui->sale_cash__radioButton->isChecked())
    {
        method = ui->sale_cash__radioButton->text();
    }else
    {
        method = "not specified";
    }
    float payAmount = ui->sale_show_paid_cash->text().toFloat();
    QString accountNumber = ui->sale_show_accnt_No->text();
    QString paidNumber = ui->sale_show_paid_number->text();
    float change = ui->sale_lcd_change->value();
    float Total = ui->sale_lcd_total->value();
    QString custName = ui->txt_customer_name->text();
    QString custPhone = ui->txt_customer_phone->text();
    float DailySales = ui->sale_lcd_daily_sale->value();

    //calling method to handle sales in sales_Table in the database
    handler.sales(sales_iD, batchCode, itemName, category, quantityStatus, quantity, Sellingprice, recommend, shell, description, remains, expireDate, tax, discount, grosstotal, method, payAmount, accountNumber, paidNumber, change, Total, custName, custPhone, DailySales);
}

