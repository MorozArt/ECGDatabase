#include "mainwindow.h"
#include "globalvariable.h"
#include <QApplication>

QString DATABASE_PATH = "/data/ECGMainDatabase.sqlite";
QString IMAGES_FORMAT = "png|jpg|jpeg";
QString ERROR_TITLE = "Ошибка!";
QString READY_TITLE = "Готово!";
int scrollValue{10};
int scrollMouseValue{4};
int axisXRange{600};
int axisXTimeRange{10};
int axisXTimeRangeScale{1};
int axisXRangeScale{axisXRange/axisXTimeRange};
double globalTimeStart{0};
double zoomValue=0.1;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
