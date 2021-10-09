#include "mainwindow.h"

#include <QApplication>
#include "logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    logger_init();
    logger_instance()->info("****************************");
    logger_instance()->info("cmcc-auto-order started");

    MainWindow w;
    w.show();

    return a.exec();
}
