#include <QDebug>
#include <QtXml>
#include "referencemanager.h"

QFile ReferenceManager::tempFile;

ReferenceManager::ReferenceManager()
{

}

bool ReferenceManager::prepareFile() {
    QSettings settings("database.conf", QSettings::IniFormat);
    setTempFile(settings.value("TempFilePath", "").toString());
    if(tempFile.exists()) {
        tempFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        tempFile.write("");
        return true;
    } else {
        return false;
    }
}

bool ReferenceManager::writeRef(QList<QPair<QString, QString> > list) {
    if(prepareFile()) {
        QDomDocument doc("References");
            QDomElement  domElement = doc.createElement("References");
            doc.appendChild(domElement);

            QDomElement itemElement;
            for(int i=0;i<list.size();++i) {
                itemElement = doc.createElement("Reference");
                itemElement.setAttribute("type", list.at(i).first);
                itemElement.setAttribute("ref", list.at(i).second);
                domElement.appendChild(itemElement);
            }

            QTextStream ts(&tempFile);
            ts.setCodec(QTextCodec::codecForName("UTF-8"));
            ts << doc.toString();
            tempFile.close();
            return true;
    } else {
        return false;
    }
}
