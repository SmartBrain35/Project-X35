#include "index.h"
#include "ui_index.h"
#include "login.h"

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

    //******************connect buttons with the slot function to switch between the pages
    QObject::connect(ui->btn_next, &QPushButton::clicked, this, &Index::graphicPageNavigation);
    QObject::connect(ui->btn_back, &QPushButton::clicked, this, &Index::graphicPageNavigation);

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
}

Index::~Index()
{
    delete ui;
}

//text browsing from sale page while typing drug name
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
    ui->txt_customer_id->clear();
    ui->txt_customer_name->clear();
    ui->txt_customer_address->clear();
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

    //testing expiring date with read out message
    QDate curDate = QDate::currentDate();
    if(curDate == curDate)
    {
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

//read out expire message
void Index::ReadLogMessage(QString log)
{
    ui->volume_slider->setValue(80); //set volume to 80%
    ui->rate_slider->setValue(1); // increase rate of speaking
    ui->pitch_slider->setValue(5); // set pitch to normall
    ui->voice->setCurrentIndex(0); //select female voice
    ui->log_message->setPlainText(log);//the message or alert
    m_speech->say(ui->log_message->toPlainText());//voice out the alert
}


void Index::on_btn_connect_clicked()
{
    //database class
    QSqlDatabase mdb = QSqlDatabase::addDatabase("MYSQL");
    mdb.setHostName("localhost");
    mdb.setDatabaseName("pharmacyscript");
    mdb.setPort(3300);
    mdb.setUserName("root");
    mdb.setPassword("smartgirl@35");

    if(mdb.open())
    {
        qDebug("connection has established successfuly");
    }else

        qDebug("connection failed to established");
}



