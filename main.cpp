#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(a.arguments());

    //w.show();

    w.showMaximized();

    return a.exec();
}
