#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <QDebug>
#include <QSplashScreen>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/images/init.png");
    QSplashScreen screen(pixmap);
    screen.show();

    MainWindow w;
    w.show();

    screen.finish(&w);

    return a.exec();
}
