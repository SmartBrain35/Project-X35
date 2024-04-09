#include "login.h"
#include "ui_login.h"
#include "index.h"

Login::Login(QWidget *parent) : QWidget(parent), ui(new Ui::Login)
{
    ui->setupUi(this);

    //set background image
    ui->bg_pic1->setPixmap(QPixmap("D:/Projects/AppDev/XNXX_35/images/n33.jpg"));

    //making the stock form framless
    setWindowFlags(Qt::FramelessWindowHint);

    //hidding login frame on app run
    ui->login_frame->setHidden(true);

    //setting the range and the initial valu03.e of the progressBar
    ui->flash_progress->setRange(1, 100);
    ui->flash_progress->setValue(1);

    //creating timer object to update the progressBar
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]()
    {
        int CurrentValue = ui->flash_progress->value();

        if(CurrentValue < 100)
        {
            ui ->flash_progress->setValue(CurrentValue + 1);
            if(CurrentValue == 20)
            {
                ui->lbl_loading_text->setText("Initializing system....");
            }if(CurrentValue == 40)
            {
                ui->lbl_loading_text->setText("connecting database....");
            }if(CurrentValue == 60)
            {
                ui->lbl_loading_text->setText("Almost there....");
            }if(CurrentValue == 90)
            {
                ui->lbl_loading_text->setText("Setup finished....");
            }

        }else
        {
            timer->stop();//stop timer when progressBar reach 100

            //open login window when progressBar reach 100% while some widgets are being hidden
            ui->login_frame->show();
            ui->flash_progress->setHidden(true);
            ui->lbl_loading_text->setHidden(true);
            ui->line_2->setHidden(true);
            ui->label_4->setHidden(true);
            ui->label_5->setHidden(true);
            ui->label_3->setHidden(true);
        }
    });

    //timer interval in milliseconds
    timer->setInterval(100);
    timer->start();//begin timer



    //adding items to the comboBox
    QStringList list ={"--default--", "Employee", "Admin", "Supervisor"};
    ui->cmb_status->addItems(list);
    ui->cmb_status->setCurrentIndex(0);

}



//exiting login form
void Login::on_btn_exit_clicked()
{
    qApp->exit();
}

//email and password validation(pull them from the database)
void Login::on_btn_login01_clicked()
{
    static Index *index = nullptr; //creating an object for the homepage and use it to access it when credentials are right

    if(!index)
    {
        index = new Index();
    }
    close();
    index->show();
}

//function to hide some widget on the login form
//will be needed when log out button is clicked
void Login::hideLoginWidget()
{
    ui->flash_progress->setHidden(true);
    ui->lbl_loading_text->setHidden(true);
    ui->label_4->setHidden(true);
    ui->label_5->setHidden(true);
    ui->label_5->setHidden(true);
    ui->line_2->setHidden(true);
    ui->label_3->setHidden(true);
    ui->label_10->setHidden(true);
    ui->login_frame->setHidden(false);
}


Login::~Login()
{
    delete ui;
}
