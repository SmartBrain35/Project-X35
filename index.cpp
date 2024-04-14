#include "index.h"
#include "ui_index.h"
#include "login.h"

Index::Index(QWidget *parent): QWidget(parent), ui(new Ui::Index)
{
    ui->setupUi(this);

    //detecting screen size and fill the form wholy with it
    setContentsMargins(0, 0, 0, 0);
    QDesktopWidget desktop;
    QRect screen = desktop.availableGeometry(this);
    setFixedSize(screen.width(), screen.height());

    //making the stock form framless
    setWindowFlags(Qt::FramelessWindowHint);

    ui->stackedWidget->setHidden(true);//start with empty homepage
    //disable homepage buttons
    ui->btn_sales->setDisabled(true);
    ui->btn_Stock->setDisabled(true);
    ui->btn_settings->setDisabled(true);
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
    //codes ended...


    //****************navigation buttons to switch the pages using lamda expression*********************
    QObject::connect(ui->btn_sales, &QPushButton::clicked, this, [=](){navigatePages(0);});
    QObject::connect(ui->btn_settings, &QPushButton::clicked, this, [=](){navigatePages(1);});
    QObject::connect(ui->btn_Stock, &QPushButton::clicked, this, [=](){navigatePages(2);});
    //QObject::connect(ui->btn_statistics, &QPushButton::clicked, this, [=](){navigatePages(3);});
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
}

//clear payment method inputs
void Index::on_sale_btn_discard_clicked()
{
    ui->sale_show_discount->clear();
    ui->sale_show_paid_cash->clear();
    ui->sale_show_paid_number->clear();
    ui->sale_show_accnt_No->clear();
}

//clear all input boxes and prepare them for th next record insertion
void Index::on_sale_btn_new_records_clicked()
{
    ui->sale_txt_batchCode->clear();
    ui->sale_drug_name->clear();
    ui->sale_quantity_counter->clear();
    ui->sale_txt_id->clear();
    ui->sale_txt_name->clear();
    ui->sale_txt_phone->clear();
    ui->sale_txt_address->clear();
}

//radio_button to start software whiles it enable some widgets
void Index::on_engine_On_clicked()
{
    ui->stackedWidget->setHidden(false);
    ui->btn_sales->setDisabled(false);
    ui->btn_Stock->setDisabled(false);
    ui->btn_settings->setDisabled(false);
    ui->btn_statistics->setDisabled(false);
    ui->stackedWidget->setCurrentIndex(0); //open sales form first
    ui->my_sales_tabs->setCurrentIndex(0);
}

//radio_button to clear homepage whiles it disable some widgets
void Index::on_engine_Off_clicked()
{
    ui->stackedWidget->setHidden(true);
    ui->btn_sales->setDisabled(true);
    ui->btn_Stock->setDisabled(true);
    ui->btn_settings->setDisabled(true);
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
