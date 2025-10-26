#include "interface.h"
#include <QApplication>
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    TInterface app;
    app.show();

    return a.exec();

}

