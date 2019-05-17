#include "dao.h"
#include "globalvariable.h"

DAO::DAO()
{

}

bool DAO::connectionToDatabase(QString dbName) {
    mainDatabase = QSqlDatabase::addDatabase("QSQLITE");
    mainDatabase.setDatabaseName(dbName);
    if (mainDatabase.open()){
        return true;
    } else {
        return false;
    }
}

bool DAO::connectionToDatabase(QString dbName, QString connectionName) {
    mainDatabase = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    mainDatabase.setDatabaseName(dbName);
    if (mainDatabase.open()){
        return true;
    } else {
        return false;
    }
}

CurrentSignal DAO::getSignal(int ratId) {
    CurrentSignal currentSignal;

    QSqlQuery a_query(mainDatabase);
    QString str = "SELECT dtu_files_link_id, result_files_link_id, rat_number, description, research_institute_id FROM main WHERE rowid = %1";
    QString str_r = str.arg(ratId);
    a_query.exec(str_r);

    int RI_id{-1};

    if(a_query.first()) {
        currentSignal.setRatId(ratId);
        currentSignal.setDtuId(a_query.value(0).toInt());
        currentSignal.setResultId(a_query.value(1).toInt());
        currentSignal.setRatNumber(a_query.value(2).toInt());
        currentSignal.setRatDescription(a_query.value(3).toString());
        RI_id = a_query.value(4).toInt();
    }

    str = "SELECT name FROM research_institute WHERE rowid = %1";
    str_r = str.arg(RI_id);
    a_query.exec(str_r);
    if(a_query.first()) {
        currentSignal.setRatRI(a_query.value(0).toString());
    }

    currentSignal.setCallouts(getTimeMarks(ratId));

    return currentSignal;
}

QString DAO::getDtuFilePath(int id, QString type) {
    QSqlQuery a_query(mainDatabase);
    QString str = "SELECT %1 FROM dtu WHERE rowid = %2";
    QString str_r = str.arg(type)
                       .arg(id);
    a_query.exec(str_r);
    if(a_query.first()) {
        return a_query.value(0).toString();
    }

    return "";
}

QString DAO::getResult(QString type, int id) {
    QSqlQuery a_query(mainDatabase);
    QString str = "SELECT %1 FROM result WHERE rowid = %2";
    QString str_r = str.arg(type)
                       .arg(id);
    a_query.exec(str_r);
    if(a_query.first()) {
        return a_query.value(0).toString();
    }

    return "";
}

QList<QString> DAO::getOpenAllRat() {
    QList<QString> items;

    QSqlQuery a_query(mainDatabase);
    a_query.exec("SELECT rat_number, research_institute_id, description FROM main");
    while(a_query.next()) {
        items.append(a_query.value(0).toString());
        items.append(a_query.value(1).toString());
        items.append(a_query.value(2).toString());
    }

    QString str = "SELECT name FROM research_institute WHERE rowid = %1";
    for(int i=1;i<items.count()-1;i+=3) {
        QString str_r = str.arg(items[i]);
        a_query.exec(str_r);
        if(a_query.first()) {
            items.replace(i, a_query.value(0).toString());
        } else {
            items.replace(i, "Неизвестно");
        }
    }

    return items;
}

QList<QString> DAO::getOpenRat(int number, QString RI) {
    QList<QString> items;

    QSqlQuery a_query(mainDatabase);
    if(RI == "Все" && number == 0) {
        return getOpenAllRat();
    }
    if(RI == "Все" && number != 0) {
        QString str = "SELECT  research_institute_id, description FROM main WHERE rat_number = %1";
        QString str_r = str.arg(number);
        a_query.exec(str_r);

        while(a_query.next()) {
            items.append(QString::number(number));
            items.append(a_query.value(0).toString());
            items.append(a_query.value(1).toString());
        }

        for(int i=1;i<items.count()-1;i+=3) {
            items.replace(i, getResearchInstituteFromId(items[i]));
        }
        return items;
    }

    int RI_id{-1};
    QString str = "SELECT rowid FROM research_institute WHERE name = '%1'";
    QString str_r = str.arg(RI);
    a_query.exec(str_r);
    if(a_query.first()) {
        RI_id = a_query.value(0).toInt();
    }

    if(number != 0) {
        str = "SELECT description FROM main WHERE rat_number = %1 AND research_institute_id = %2";
        str_r = str.arg(number)
                   .arg(RI_id);
        a_query.exec(str_r);

        while(a_query.next()) {
            items.append(QString::number(number));
            items.append(RI);
            items.append(a_query.value(0).toString());
        }
        return items;
    } else {
        str = "SELECT rat_number, description FROM main WHERE research_institute_id = %1";
        str_r = str.arg(RI_id);
        a_query.exec(str_r);

        while(a_query.next()) {
            items.append(a_query.value(0).toString());
            items.append(RI);
            items.append(a_query.value(1).toString());
        }
        return items;
    }
}

QList<QPair<int, QString> > DAO::getResearchInstitutes() {
    QList<QPair<int, QString> > items;

    QSqlQuery a_query(mainDatabase);
    a_query.exec("SELECT rowid, name FROM research_institute");
    QPair<int, QString> item;
    while(a_query.next()) {
        item.first = a_query.value(0).toInt();
        item.second = a_query.value(1).toString();
        items.append(item);
    }
    return items;
}

QList<QPair<QString,QString>> DAO::getAllRecordForResearchInstitute(int RIid) {
    QList<QPair<QString, QString> > items;

    QSqlQuery a_query(mainDatabase);
    QString str = "SELECT rat_number, description FROM main WHERE research_institute_id = %1";
    QString str_r = str.arg(RIid);
    a_query.exec(str_r);
    QPair<QString, QString> item;
    while(a_query.next()) {
        item.first = a_query.value(0).toString();
        item.second = a_query.value(1).toString();
        items.append(item);
    }
    return items;
}

QString DAO::getResearchInstituteFromId(QString id) {
    QSqlQuery a_query(mainDatabase);
    QString str = "SELECT name FROM research_institute WHERE rowid = %1";
    QString str_r = str.arg(id);
    a_query.exec(str_r);
    if(a_query.first()) {
        return a_query.value(0).toString();
    } else {
        return "Неизвестно";
    }
}

int DAO::getResearchInstituteFromName(QString name) {
    QSqlQuery a_query(mainDatabase);
    QString str = "SELECT rowid FROM research_institute WHERE name = '%1'";
    QString str_r = str.arg(name);
    a_query.exec(str_r);
    if(a_query.first()) {
        return a_query.value(0).toInt();
    } else {
        return -1;
    }
}

QList<QString> DAO::getAllResultType(QString type) {
    QList<QString> items, temp;
    QSqlQuery a_query(mainDatabase);
    QString str = "SELECT %1, rowid FROM result";
    QString str_r = str.arg(type);
    a_query.exec(str_r);

    while(a_query.next()) {
        temp.append(a_query.value(0).toString());
        temp.append(a_query.value(1).toString());
    }
    for(int i=1;i<temp.count();i+=2) {
        QString str = "SELECT rat_number, research_institute_id, description FROM main WHERE rowid = %1";
        QString str_r = str.arg(temp[i]);
        a_query.exec(str_r);
        if(a_query.first()) {
            items.append(a_query.value(0).toString()); //rat_number
            items.append(getResearchInstituteFromId(a_query.value(1).toString())); //research_institute
            items.append(a_query.value(2).toString()); //description
            items.append(temp[i-1]); //Path
        }
    }
    return items;
}

QList<QString> DAO::getResultType(QString type, int ratNumber, QString RI) {
    QList<QString> items, temp;
    temp = getAllResultType(type);
    if((ratNumber != 0) && (RI != "Все")) {
        for(int i=0;i<temp.count();i+=4) {
            if((temp[i].toInt() == ratNumber) && (temp[i+1] == RI)) {
                items.append(temp[i]);
                items.append(temp[i+1]);
                items.append(temp[i+2]);
                items.append(temp[i+3]);
            }
        }
        return items;
    }

    if((ratNumber != 0) && (RI == "Все")) {
        for(int i=0;i<temp.count();i+=4) {
            if(temp[i].toInt() == ratNumber) {
                items.append(temp[i]);
                items.append(temp[i+1]);
                items.append(temp[i+2]);
                items.append(temp[i+3]);
            }
        }
        return items;
    }

    if((ratNumber == 0) && (RI != "Все")) {
        for(int i=0;i<temp.count();i+=4) {
            if(temp[i+1] == RI) {
                items.append(temp[i]);
                items.append(temp[i+1]);
                items.append(temp[i+2]);
                items.append(temp[i+3]);
            }
        }
        return items;
    }
    return items;
}

double timeCast(double time) {
    int timeResidue{0};
    timeResidue = (static_cast<int>(time*100))%100;
    if(timeResidue != 0) {
        time = (static_cast<int>(time*100) - timeResidue)/100;
        time += static_cast<double>(timeResidue)/60;
    }
    return time;
}

QList<QPair<double, QString> > DAO::getTimeMarks(int id) {
    QList<QPair<double, QString> > timeMarks;
    QSqlQuery a_query(mainDatabase);
    QString str = "SELECT time_description FROM main WHERE rowid = %1";
    QString str_r = str.arg(id);
    a_query.exec(str_r);
    if(a_query.first()) {
        str = a_query.value(0).toString();
    }

    QDomDocument domDoc;
    QFile file(str);
    if(file.open(QIODevice::ReadOnly)) {
        if(domDoc.setContent(&file)) {
            QDomElement domElement = domDoc.documentElement();
            if(domElement.tagName() == "rat") {
                globalTimeStart = timeCast(domElement.attribute("start_global_time","").toDouble());

                QDomNode domNode = domElement.firstChild();
                while(!domNode.isNull()) {
                    if(domNode.isElement()) {
                        QDomElement timeMarkElement = domNode.toElement();
                        if(!timeMarkElement.isNull() && timeMarkElement.tagName() == "time_mark") {
                            QPair<double, QString> timeMark;
                            timeMark.first = 60*timeCast(timeMarkElement.attribute("time","").toDouble());
                            timeMark.second = QString::fromStdString(timeMarkElement.attribute("desc","").toStdString());
                            timeMarks.append(timeMark);
                        }
                    }
                    domNode = domNode.nextSibling();
                }
            }
        }
        file.close();
    }

    return timeMarks;
}

int DAO::getRatId(int ratNumber, int RIid, QString ratDesc) {
    QSqlQuery a_query(mainDatabase);
    QString str = "SELECT rowid FROM main WHERE rat_number = %1 AND research_institute_id = %2 AND description = '%3'";
    QString str_r = str.arg(ratNumber)
                       .arg(RIid)
                       .arg(ratDesc);
    a_query.exec(str_r);
    if(a_query.first()) {
        return a_query.value(0).toInt();
    } else {
        return -1;
    }
}

void DAO::insertECGRecord(int ratNumber, QString ratDesc, int RIid) {
    QSqlQuery a_query(mainDatabase);
    QString str = "INSERT INTO main(rat_number,description,research_institute_id) " "VALUES (%1, '%2', %3);";
    QString str_r = str.arg(ratNumber)
                       .arg(ratDesc)
                       .arg(RIid);
    a_query.exec(str_r);
}

void DAO::insertZetLabFiles(int ratId, QString zetLabDirPath) {
    QSqlQuery a_query(mainDatabase);
    QString str = "UPDATE main SET zetlab_files = '%1' WHERE rowid = %2";
    QString str_r = str.arg(zetLabDirPath)
                       .arg(ratId);
    a_query.exec(str_r);
}

void DAO::insertDtuFiles(int ratId, QString LF1, QString LF2, QString LF3, QString HF1, QString HF2, QString HF3) {
    QSqlQuery a_query(mainDatabase);
    QString str = "INSERT INTO dtu(LF1, LF2, LF3, HF1, HF2, HF3) " "VALUES ('%1', '%2', '%3', '%4', '%5', '%6');";
    QString str_r = str.arg(LF1)
                       .arg(LF2)
                       .arg(LF3)
                       .arg(HF1)
                       .arg(HF2)
                       .arg(HF3);
    a_query.exec(str_r);

    str = "SELECT rowid FROM dtu WHERE LF1 = '%1' AND HF1 = '%2'";
    str_r = str.arg(LF1)
               .arg(HF1);
    a_query.exec(str_r);
    if(a_query.first()) {
        str = "UPDATE main SET dtu_files_link_id = %1 WHERE rowid = %2";
        str_r = str.arg(a_query.value(0).toInt())
                   .arg(ratId);
        a_query.exec(str_r);
    }
}

void DAO::insertFile(int id, QString filePath, QString type) {
    QSqlQuery a_query(mainDatabase);
    QString table{""};
    QString field{""};
    if(type == "xml") {table = "main "; field = "time_description";}
    if(type == "keyPoints") {table = "result "; field = "keypoints";}
    QString str = "UPDATE "+table+"SET "+field+" = '%1' WHERE rowid = %2";
    QString str_r = str.arg(filePath)
                       .arg(id);
    a_query.exec(str_r);

}

void DAO::insertResult(int resId, QString resultDirPath, QString type) {
    QSqlQuery a_query(mainDatabase);
    QString field{""};
    if(type == "waveform") field = "waveform";
    if(type == "wavelet") field = "wavelet";
    if(type == "histogram") field = "histogram";
    if(type == "spectrum") field = "spectrum";
    QString str = "UPDATE result SET "+field+" = '%1' WHERE rowid = %2";
    QString str_r = str.arg(resultDirPath)
                       .arg(resId);
    a_query.exec(str_r);
}

void DAO::closeConnection(QString dbName) {
    mainDatabase.close();
    QSqlDatabase::removeDatabase(dbName);
}

int DAO::createResultId(int ratId) {
    int resultId{-1};
    QSqlQuery a_query(mainDatabase);
    QString str = "INSERT INTO result DEFAULT VALUES";
    a_query.exec(str);
    str = "SELECT rowid FROM result WHERE rowid = last_insert_rowid()";
    a_query.exec(str);
    if(a_query.first()) {
        resultId = a_query.value(0).toInt();
    }

    str = "UPDATE main SET result_files_link_id = %1 WHERE rowid = %2";
    QString str_r = str.arg(resultId)
                       .arg(ratId);
    a_query.exec(str_r);

    return resultId;
}

QList<QPair<QString, QString> > DAO::getZetLabRef(int channel, int ratId) {
    QList<QPair<QString, QString> > list;
    QPair<QString, QString> pair;
    QSqlQuery a_query(mainDatabase);
    QString str = "SELECT zetlab_files FROM main WHERE rowid = %1";
    QString str_r = str.arg(ratId);
    a_query.exec(str_r);

    if(a_query.first()) {
        QStringList files = QDir(a_query.value(0).toString()).entryList();
        QRegExp expAna("^sig000("+QString::number(channel)+")\\.(ana)$"),
                expAnp("^sig000("+QString::number(channel)+")\\.(anp)$"),
                expXml("^sig000("+QString::number(channel)+")\\.(xml)$");
        for(int i=0;i<files.size();++i) {
            if(expAna.indexIn(files.at(i)) != -1) {
                pair.first = "ana";
                pair.second = a_query.value(0).toString() + "\\" + files.at(i);
                list.append(pair);
            }

            if(expAnp.indexIn(files.at(i)) != -1) {
                pair.first = "anp";
                pair.second = a_query.value(0).toString() + "\\" + files.at(i);
                list.append(pair);
            }

            if(expXml.indexIn(files.at(i)) != -1) {
                pair.first = "xml";
                pair.second = a_query.value(0).toString() + "\\" + files.at(i);
                list.append(pair);
            }
        }
    }

    return list;
}

QList<QPair<QString, QString> > DAO::getWaveformRef(int channel, int resultId) {
    QList<QPair<QString, QString> > list;
    QPair<QString, QString> pair;
    QSqlQuery a_query(mainDatabase);
    QString str = "SELECT waveform FROM result WHERE rowid = %1";
    QString str_r = str.arg(resultId);
    a_query.exec(str_r);

    if(a_query.first()) {
        QStringList files = QDir(a_query.value(0).toString()).entryList();
        QRegExp exp("^waveform ("+QString::number(channel)+")\\.("+IMAGES_FORMAT+")$");
        for(int i=0;i<files.size();++i) {
            if(exp.indexIn(files.at(i)) != -1) {
                pair.first = "waveform";
                pair.second = a_query.value(0).toString() + "\\" + files.at(i);
                list.append(pair);
            }
        }
    }

    return list;
}

void DAO::deleteECGRecord(int ratId) {
    int resultId{-1}, dtuId{-1};
    QSqlQuery a_query(mainDatabase);

    QString str = "SELECT result_files_link_id FROM main WHERE rowid = %1";
    QString str_r = str.arg(ratId);
    a_query.exec(str_r);
    if(a_query.first()) {
        resultId = a_query.value(0).toInt();
    }

    str = "SELECT dtu_files_link_id FROM main WHERE rowid = %1";
    str_r = str.arg(ratId);
    a_query.exec(str_r);
    if(a_query.first()) {
        dtuId = a_query.value(0).toInt();
    }

    if(resultId != -1) {
        str = "DELETE FROM result WHERE rowid = %1";
        str_r = str.arg(resultId);
        a_query.exec(str_r);
    }

    if(dtuId != -1) {
        str = "DELETE FROM dtu WHERE rowid = %1";
        str_r = str.arg(dtuId);
        a_query.exec(str_r);
    }

    str = "DELETE FROM main WHERE rowid = %1";
    str_r = str.arg(ratId);
    a_query.exec(str_r);
}

void DAO::deleteResearchInstitute(int RIid) {
    QSqlQuery a_query(mainDatabase);
    QString str = "DELETE FROM research_institute WHERE rowid = %1";
    QString str_r = str.arg(RIid);
    a_query.exec(str_r);
}

DAO::~DAO() {
    mainDatabase.close();
}
