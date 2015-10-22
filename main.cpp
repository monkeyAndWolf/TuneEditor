#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QIcon icon(":/icons/kantele.png");
    a.setWindowIcon(icon);

    MainWindow w;
    w.show();

    return a.exec();
}
