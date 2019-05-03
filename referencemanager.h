#ifndef REFERENCEMANAGER_H
#define REFERENCEMANAGER_H

#include <QList>

class ReferenceManager
{
public:
    ReferenceManager();
    static bool writeRef(QList< QPair<QString,QString> > list);
    static void setTempFile(QString pathToTemp) {tempFile.setFileName(pathToTemp);}

private:
    static bool prepareFile();
    static QFile tempFile;
};

#endif // REFERENCEMANAGER_H
