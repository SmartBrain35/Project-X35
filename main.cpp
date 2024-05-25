#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "index.h"
#include "login.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //linking css file i.e part_1_StyleSheet.css to the login UI
    QFile beauty_file(":/new/CSS/Style.css"); //load the file css file

    if(!beauty_file.open(QIODevice::ReadOnly | QIODevice::Text)) //if open & read successful
    {
        qWarning("css file can not be read");

        return 1;
    }

    QTextStream stream(&beauty_file); //convert to text stream
    QString css = stream.readAll(); //then app invokes it
    beauty_file.close(); //close opened css file

    qApp->setStyleSheet(css);

    Index app; //first thing to appear when the app run is the splash form
    app.show();

    return a.exec();
}
