#include "receivegraphictask.h"

ReceiveGraphicTask::ReceiveGraphicTask(QObject *parent) : QObject(parent)
{

}

ReceiveGraphicTask::ReceiveGraphicTask(int dtuId, QString type)
{
    this->dtuId = dtuId;
    this->type = type;
}

void ReceiveGraphicTask::doWork() {
    if(dao.connectionToDatabase(dbName, "thread")){
        QVector<QPair<double, double>> ser;
        if(getSeries(ser, type)) {
            QLineSeries *series = RamerDouglasPeuckerAlgorithm(ser);
            emit send(series);
        }
    }


    /*QLineSeries *series = new QLineSeries();
    for(int i=0;i<1800;++i) {
        series->append(i,rand());
    }
    emit send(series);*/


    emit finished();
}

double ReceiveGraphicTask::getDistance(QPair<double, double> start, QPair<double, double> end, QPair<double, double> point) {
    return ((abs((end.second - start.second)*point.first - (end.first - start.first)*point.second
                 + end.first*start.second - end.second*start.first))/
            (sqrt(pow((end.second-start.second),2) + pow((end.first-start.first),2))));
}

void ReceiveGraphicTask::simplify(int start, int end, QVector<QPair<double, double> > &ser, QList<int> &result) {
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

    if(maxd > 0.5) {
        result.append(maxi);
        simplify(start, maxi, ser, result);
        simplify(maxi, end, ser, result);
    }
}

QLineSeries *ReceiveGraphicTask::RamerDouglasPeuckerAlgorithm(QVector<QPair<double, double>> &ser) {
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

bool ReceiveGraphicTask::getSeries(QVector<QPair<double, double> > &ser, QString type) {

    QString str = dao.getDtuFilePath(dtuId, type);
    if(QFile::exists(str)) {
        QFile file(str);
        if(file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            QString str;
            while(stream.readLine() != "c	mV") {}
            str = stream.readLine();
            QPair<double, double> temp;
            while(str != nullptr) {
                temp.first = str.split('\t').first().toDouble();
                temp.second = str.split('\t').last().toDouble();
                ser.append(temp);
                str = stream.readLine();
                str = stream.readLine();// Под БОЛЬШИМ вопросом
            }
        }
        return true;
    } else {
        return false;
    }
}
