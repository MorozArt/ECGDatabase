#ifndef ECGGRAPHICDRAWER_H
#define ECGGRAPHICDRAWER_H

#include <QtCharts>
#include <QChartView>
#include "ecgchartview.h"

class ECGGraphicDrawer
{
public:
    ECGGraphicDrawer();
    ~ECGGraphicDrawer() {if(chart != nullptr) delete chart;}
    void drowGraphic(ECGChartView *view, QLineSeries *series, QLineSeries *timeAxis,
                     QLineSeries *globalTimeAxis, QList<QLineSeries*> timeMarks);
    QLineSeries *RamerDouglasPeuckerAlgorithm(QVector<QPair<double, double>> &ser);
    QChart *getChart() {return chart;}
private:
    QChart *chart = nullptr;
    void simplify(int start, int end, QVector<QPair<double, double>> &ser, QList<int> &result);
    double getDistance(QPair<double, double> start, QPair<double, double> end, QPair<double, double> point);
};

#endif // ECGGRAPHICDRAWER_H
