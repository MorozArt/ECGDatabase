#ifndef DAO_H
#define DAO_H

#include <QtSql>
#include <QtXml>
#include "currentsignal.h"
#include "callout.h"


class DAO
{
public:
    DAO();
    ~DAO();
    bool connectionToDatabase(QString dbName);
    bool connectionToDatabase(QString dbName, QString connectionName);
    CurrentSignal getSignal(int ratId);
    QString getDtuFilePath(int id, QString type);
    QString getResult(QString type, int id);
    QString getResearchInstituteFromId(QString id);
    QList<QString> getOpenAllRat();
    QList<QString> getOpenRat(int number, QString RI);
    QList<QPair<int,QString>> getResearchInstitutes();
    QList<QPair<QString,QString>> getAllRecordForResearchInstitute(int RIid);
    QList<QString> getAllResultType(QString type);
    QList<QString> getResultType(QString type, int ratNumber, QString RI);
    QList<QPair<double, QString> > getTimeMarks(int id);
    QList<QPair<QString, QString> > getZetLabRef(int channel, int ratId);
    QList<QPair<QString, QString> > getWaveformRef(int channel, int resultId);
    int getRatId(int ratNumber, int RIid, QString ratDesc);
    int createResultId(int ratId);
    int getResearchInstituteFromName(QString name);
    void insertECGRecord(int ratNumber, QString ratDesc, int RIid);
    void insertZetLabFiles(int ratId, QString zetLabDirPath);
    void insertDtuFiles(int ratId, QString LF1, QString LF2, QString LF3, QString HF1, QString HF2, QString HF3);
    void insertFile(int ratId, QString filePath, QString type);
    void insertResult(int resId, QString resultDirPath, QString type);
    void deleteECGRecord(int ratId);
    void deleteResearchInstitute(int RIid);
    void closeConnection(QString dbName);
private:
    QSqlDatabase mainDatabase;
};

#endif // DAO_H
