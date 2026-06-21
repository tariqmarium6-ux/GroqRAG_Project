#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // 1. App ka Engine start karo
    QApplication a(argc, argv);

    // 2. Front Desk (GUI) create karo
    MainWindow w;

    // 3. Screen par show karo
    w.show();

    // 4. App ko on rakho jab tak user cross(it pauses the process) till user (X) na dabaye
    return a.exec();
}