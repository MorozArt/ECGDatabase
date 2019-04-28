#include <QDebug>
#include <QtXml>
#include "referencemanager.h"

ReferenceManager::ReferenceManager()
{

}

bool ReferenceManager::prepareFile() {
    QFile file("../temp.xml");
    if(file.exists()) {
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        file.write("");
        file.close ();
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

            QFile file("../temp.xml");
            if(file.open(QIODevice::WriteOnly)) {
                QTextStream ts(&file);
                ts.setCodec(QTextCodec::codecForName("UTF-8"));
                ts << doc.toString();
                file.close();
                return true;
            } else {
                return false;
            }

    } else {
        return false;
    }
}
