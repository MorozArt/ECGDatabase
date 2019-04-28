#ifndef REFERENCEMANAGER_H
#define REFERENCEMANAGER_H

#include <QList>

class ReferenceManager
{
public:
    ReferenceManager();
    static bool writeRef(QList< QPair<QString,QString> > list);

private:
    static bool prepareFile();
};

#endif // REFERENCEMANAGER_H
