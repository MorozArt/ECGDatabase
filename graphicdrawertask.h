#ifndef GRAPHICDRAWERTASK_H
#define GRAPHICDRAWERTASK_H

#include <QObject>
#include <QtCharts>
#include "dao.h"

extern QString DATABASE_PATH;

class GraphicDrawerTask : public QObject
{
    Q_OBJECT
public:
    explicit GraphicDrawerTask(QObject *parent = 0);
    GraphicDrawerTask(int dtuId, QString type);
    ~GraphicDrawerTask() {dao.closeConnection(dbName);}
public slots:
    void doWork();
signals:
    void send(QLineSeries*);
    void finished();
private:
    double getDistance(QPair<double, double> start, QPair<double, double> end, QPair<double, double> point);
    void simplify(int start, int end, QVector<QPair<double, double> > &ser, QList<int> &result);
    QLineSeries *RamerDouglasPeuckerAlgorithm(QVector<QPair<double, double>> &ser);
    bool getSeries(QVector<QPair<double, double>> &ser, QString type);
    DAO dao;
    int dtuId;
    QString type{""};
    QString dbName{qApp->applicationDirPath()+DATABASE_PATH};
    bool firstIteration{true};
};

#endif // GRAPHICDRAWERTASK_H
