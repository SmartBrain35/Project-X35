#include "index.h"
#include "ui_index.h"

Index::Index(QWidget *parent): QWidget(parent), ui(new Ui::Index)
{
    ui->setupUi(this);

    //detecting screen size and fill the form wholy with it
    setContentsMargins(0, 0, 0, 0);

    //set background image
    ui->bg_pic->setPixmap(QPixmap("D:/Projects/AppDev/XNXX_35/images/n44.jpg"));

    //making the stock form framless
    setWindowFlags(Qt::FramelessWindowHint);

}

Index::~Index()
{
    delete ui;
}

