#ifndef RECEIVEGRAPHICTASK_H
#define RECEIVEGRAPHICTASK_H

#include <QObject>
#include <QtCharts>
#include "dao.h"

extern QString DATABASE_PATH;

class ReceiveGraphicTask : public QObject
{
    Q_OBJECT
public:
    explicit ReceiveGraphicTask(QObject *parent = 0);
    ReceiveGraphicTask(int dtuId, QString type);
    ~ReceiveGraphicTask() {dao.closeConnection(dbName);}
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

#endif // RECEIVEGRAPHICTASK_H
