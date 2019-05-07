#include "ecggraphicdrawer.h"
#include "globalvariable.h"

ECGGraphicDrawer::ECGGraphicDrawer()
{

}

void ECGGraphicDrawer::drowGraphic(ECGChartView *view, QLineSeries *series, QLineSeries *timeAxis,
                                   QLineSeries *globalTimeAxis, QList<QLineSeries *> timeMarks) {
    chart = new QChart();

    chart->legend()->hide();
    timeAxis->setColor(QColor(1,0,0));
    globalTimeAxis->setColor(QColor(1,0,0));
    qDebug() << "3";
    chart->addSeries(series);
    qDebug() << "4";
    chart->addSeries(timeAxis);
    chart->addSeries(globalTimeAxis);

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, axisXRange);
    axisX->setTickAnchor(0);
    axisX->setTickInterval(60);
    axisX->setTickType(QValueAxis::TicksDynamic);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    axisX->hide();

    QValueAxis *axisTime = new QValueAxis();
    axisTime->setTitleText("Время, мин");
    axisTime->setRange(0, axisXTimeRange);
    axisTime->setLabelFormat("%3d");
    axisTime->setTickAnchor(0);
    axisTime->setTickInterval(1);
    axisTime->setTickType(QValueAxis::TicksDynamic);
    axisTime->setLinePenColor(QColor(0,0,0));
    chart->addAxis(axisTime, Qt::AlignBottom);
    timeAxis->attachAxis(axisTime);

    QValueAxis *axisGlobalTime = new QValueAxis();
    axisGlobalTime->setTitleText("Время от начала опыта, мин");
    axisGlobalTime->setRange(globalTimeStart, globalTimeStart+axisXTimeRange);
    axisGlobalTime->setLabelFormat("%3d");
    axisGlobalTime->setTickAnchor(0);
    axisGlobalTime->setTickInterval(1);
    axisGlobalTime->setTickType(QValueAxis::TicksDynamic);
    axisGlobalTime->setLinePenColor(QColor(0,0,0));
    chart->addAxis(axisGlobalTime, Qt::AlignBottom);
    globalTimeAxis->attachAxis(axisGlobalTime);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("mV");
    axisY->setTickCount(8);
    axisY->setLinePenColor(QColor(0,0,0));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    timeAxis->attachAxis(axisY);
    globalTimeAxis->attachAxis(axisY);

    for(int i=0;i<timeMarks.count();++i) {
        chart->addSeries(timeMarks[i]);
        timeMarks[i]->attachAxis(axisX);
        //timeMarks[i]->attachAxis(axisY);
        timeMarks[i]->setColor(QColor(200,100,0));
    }

    view->setChart(chart);
}

QLineSeries *ECGGraphicDrawer::RamerDouglasPeuckerAlgorithm(QVector<QPair<double, double>> &ser) {
    QLineSeries *resultSeries = new QLineSeries();
    QList<int> result;
    result.append(0);
    simplify(0, ser.length()-1, ser, result);
    result.append(ser.length()-1);
    std::sort(result.begin(), result.end());
    for(int i=0;i<result.length(); ++i) {
        resultSeries->append(ser[result[i]].first, ser[result[i]].second);
    }
    return resultSeries;
}

void ECGGraphicDrawer::simplify(int start, int end, QVector<QPair<double, double> > &ser, QList<int> &result) {
    if(start+1 == end) {
        return;
    }

    double maxd = -1;
    int maxi = end;

    for(int i = start+1; i<end; ++i) {
        double dv = getDistance(ser[start],ser[end], ser[i]);
        if(dv > maxd) {
            maxi = i;
            maxd = dv;
        }
    }

    if(maxd > 0.1) {
        result.append(maxi);
        simplify(start, maxi, ser, result);
        simplify(maxi, end, ser, result);
    }
}

double ECGGraphicDrawer::getDistance(QPair<double, double> start, QPair<double, double> end, QPair<double, double> point) {
    return ((abs((end.second - start.second)*point.first - (end.first - start.first)*point.second
                 + end.first*start.second - end.second*start.first))/
            (sqrt(pow((end.second-start.second),2) + pow((end.first-start.first),2))));
}
