#include "xs/XSApplication.h"

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // QApplication consumes the arguments it understands.
    QApplication app(argc, argv);

    XSApplication xsapp;

    MainWindow window;

    window.show();
    return app.exec();
}
