#ifndef ECGCHARTVIEW_H
#define ECGCHARTVIEW_H

#include <QtCharts>
#include "callout.h"

class ECGChartView : public QChartView
{
public:
    ECGChartView(QWidget *parent = 0);
    ECGChartView(QChart *chart, QWidget *parent = 0);
    ~ECGChartView() {}
    void setTimeMarks(QList<QPair<double, QString> > timeMarks) {this->timeMarksCallout = timeMarks;}
public slots:
    void tooltip(QPointF point, bool state);
protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    bool leftClick{false};
    QPoint mousePos;
    Callout *m_tooltip = nullptr;
    QList<QPair<double, QString> > timeMarksCallout;
};

#endif // ECGCHARTVIEW_H
