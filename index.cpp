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

    //set background image
    ui->bg_pic->setPixmap(QPixmap("D:/Projects/AppDev/XNXX_35/images/n44.jpg"));

    //ui->stackedWidget->setHidden(true);//start with empty homepage
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

}


Index::~Index()
{
    delete ui;
}


//text browsing from sale page while typing drug name
void Index::UpdateSearchDrugOutcome(const QString &text)
{
    //ui->outcome->clear();
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

