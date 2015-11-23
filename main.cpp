#include "mainwindow.h"
#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QIcon icon(":/icons/kantele.png");
    a.setWindowIcon(icon);

    QSettings *settings = new QSettings("monkey&wolf", "TuneEditor");
    MainWindow w(settings);
    if (!settings->value("dimensions").isNull())
    {
        QVariant protoDimensions = settings->value("dimensions");
        QRect geomish = protoDimensions.toRect();
        w.setGeometry(geomish);
    }
    w.show();

    return a.exec();
}
