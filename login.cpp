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

}

Login::~Login()
{
    delete ui;
}

//exiting login form
void Login::on_btn_exit_clicked()
{
    qApp->exit();
}

//email and password validation(pull them from the database)
void Login::on_btn_login01_clicked()
{
    Index index; //creating an object for the homepage and use it to access it when credentials are right
    if(1==1)
    {
        //login successfull, open the home page & close login form
        index.show();
        ui->login_frame->close();
        close(); //close existing(spflash & login) page when opening hompage
    }else
    {
        //invalid email or password
    }
}

