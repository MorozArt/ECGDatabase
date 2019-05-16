#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QPair>
#include <QLabel>
#include "dao.h"

class FileManager : public QObject
{
    Q_OBJECT

signals:
    void copyFinished();

public:
    FileManager();
    bool correctZetLabFiles(QString path, QLabel *zetLabCheklabel, QLabel *errorZetLablabel);
    bool correctFileType(QString path, QString type, QLabel *cheklabel, QLabel *errorLabel);
    bool correctResult(QString path, QString type, QLabel *cheklabel, QLabel *errorLabel);
    bool correctResultFiles(QString path, QString channel, QString type, QLabel *cheklabel, QLabel *errorLabel);
    QString createDir(QString ratNum, QString ratDesc);
    bool zetLabFilesMove(QString dirPath, QString zetLabFilesPath, int ratId);
    bool fileMove(int id, QString filePath, QString dirPath, QString type);
    bool resultDirMove(int id, QString resultDirPath, QString dirPath, QString type);
    bool runCopyTask(QString source, QString dest);
    bool copyDir(QString source, QString dest);
    bool deleteECGRecord(QString ratNumber, QString ratDesc, int ratId);
private:
    DAO dao;
};

#endif // FILEMANAGER_H
