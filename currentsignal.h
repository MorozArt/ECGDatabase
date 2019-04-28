#ifndef CURRENTSIGNAL_H
#define CURRENTSIGNAL_H

#include <QString>
#include <QtCharts>

class CurrentSignal
{
public:
    CurrentSignal();

    void setRatId(int ratId) {
        this->ratId = ratId;
    }

    int getRatId() {
        return ratId;
    }

    void setRatNumber(int ratNumber) {
        this->ratNumber = ratNumber;
    }

    int getRatNumber() {
        return ratNumber;
    }

    void setRatDescription(QString ratDescription) {
        this->ratDescription = ratDescription;
    }

    QString getRatDescription() {
        return ratDescription;
    }

    void setRatRI(QString ratRI) {
        this->ratRI = ratRI;
    }

    QString getRatRI() {
        return ratRI;
    }

    void setDtuId(int dtuId) {
        this->dtuId = dtuId;
    }

    int getDtuId() {
        return dtuId;
    }

    void setResultId(int resultId) {
        this->resultId = resultId;
    }

    int getResultId() {
        return resultId;
    }

    void setTimeMarks(QList<QLineSeries*> timeMarks) {
        this->timeMarks = timeMarks;
    }

    QList<QLineSeries*> getTimeMarks() {
        return timeMarks;
    }

    void setCallouts(QList<QPair<double, QString> > callouts) {
        this->callouts = callouts;
    }

    QList<QPair<double, QString> > getCallouts() {
        return callouts;
    }
private:
    int ratId{-1};
    int ratNumber{-1};
    QString ratDescription{""};
    QString ratRI{""};
    int dtuId{-1};
    int resultId{-1};
    QList<QLineSeries*> timeMarks;
    QList<QPair<double, QString> > callouts;
};

#endif // CURRENTSIGNAL_H
