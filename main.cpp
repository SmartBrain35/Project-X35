#include <QApplication>
#include "index.h"
#include "login.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Login app; //first thing to appear when the app run is the splash form
    app.show();

    return a.exec();
}
