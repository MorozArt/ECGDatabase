#include <QFileDialog>
#include <QApplication>
#include <QDebug>
#include "filemanager.h"
#include "globalvariable.h"

FileManager::FileManager()
{
    dao.connectionToDatabase(qApp->applicationDirPath()+DATABASE_PATH, "fileManager");
}

bool FileManager::correctZetLabFiles(QString path, QLabel *zetLabCheklabel, QLabel *errorZetLablabel) {
    bool zetLabCorrect{true};
    QDir dir(path);
    QStringList list;
    QString excessFiles{""};
    list = dir.entryList(QDir::Files);
    int filesCount{0};

    QString errorStr = "Отсутствуют: \n";
    if(!list.isEmpty()) {
        for(int i=1;i<=6;++i) {
            if(!list.contains("sig000"+QString::number(i)+".ana")) {
                zetLabCorrect = false;
                errorStr += "\tsig000"+QString::number(i)+".ana\n";
            } else ++filesCount;
            if(!list.contains("sig000"+QString::number(i)+".anp")) {
                zetLabCorrect = false;
                errorStr += "\tsig000"+QString::number(i)+".anp\n";
            } else ++filesCount;
            if(!list.contains("sig000"+QString::number(i)+".xml")) {
                zetLabCorrect = false;
                errorStr += "\tsig000"+QString::number(i)+".xml\n";
            } else ++filesCount;
            if(!list.contains("sig000"+QString::number(i)+".dtu")) {
                zetLabCorrect = false;
                errorStr += "\tsig000"+QString::number(i)+".dtu\n";
            } else ++filesCount;
        }

        if(list.size() > filesCount) {
            zetLabCorrect = false;
            QRegExp exp("^sig000[1-6]\\.(ana|anp|xml|dtu)$");
            for(int i=0;i<list.size();++i) {
                if(exp.indexIn(list.at(i)) == -1) {
                    excessFiles += (list.at(i) + "\n");
                }
            }
        }
    } else {
        errorStr = "В данной папке отсутствуют требуемые файлы!";
        zetLabCorrect = false;
    }

    if(zetLabCorrect) {
        errorZetLablabel->setText("");
        errorZetLablabel->setToolTip("");
        zetLabCheklabel->setPixmap(QPixmap(":/images/true.png"));
    } else {
        if(list.size() > filesCount) {
            errorZetLablabel->setText("Внимание! В папке присутствуют лишние файлы!");
            errorZetLablabel->setToolTip(excessFiles);
        } else {
            errorZetLablabel->setText("Ошибка!");
            errorZetLablabel->setToolTip(errorStr);
        }
        zetLabCheklabel->setPixmap(QPixmap(":/images/false.png"));
    }

    return zetLabCorrect;
}

bool FileManager::correctFileType(QString path, QString type, QLabel *cheklabel, QLabel *errorLabel) {
    bool result;

    QRegExp exp("*."+type);
    exp.setPatternSyntax(QRegExp::Wildcard);
    if(exp.exactMatch(path) && QFile::exists(path)) {
        result = true;
        cheklabel->setPixmap(QPixmap(":/images/true.png"));
        errorLabel->setText("");
    } else {
        result = false;
        cheklabel->setPixmap(QPixmap(":/images/false.png"));
        errorLabel->setText("Указан неверный файл!");
    }

    return result;
}

bool FileManager::correctResult(QString path, QString type, QLabel *cheklabel, QLabel *errorLabel) {
    bool result{true};
    QDir dir(path);
    QStringList list = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList fiList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    if(list.size() != 6 || fiList.size() != 6) {
        cheklabel->setPixmap(QPixmap(":/images/false.png"));
        errorLabel->setText("Неверное количество папок или присутствуют лишние файлы!");
        return false;
    } else {
        cheklabel->setPixmap(QPixmap(":/images/NoRequired.png"));
        errorLabel->setText("");
        for(int i=0;i<6;++i) {
            if(!correctResultFiles(path, list.at(i), type, cheklabel, errorLabel)) result = false;
        }

        if(result) {
            cheklabel->setPixmap(QPixmap(":/images/true.png"));
            errorLabel->setText("");
        } else errorLabel->setText("Ошибка!");
    }

    return result;
}

bool FileManager::correctResultFiles(QString path, QString channel, QString type, QLabel *cheklabel, QLabel *errorLabel) {
    bool result{false};

    QDir dir(path+"/"+channel);
    QStringList list = dir.entryList(QDir::Files);
    QString excessFiles{channel+":\n"};
    QString toolTipText = errorLabel->toolTip();
    int filesCount{0};
    bool correct{true};
    QRegExp exp("^("+type+")\\s\\d+\\-\\d+\\.("+IMAGES_FORMAT+"|txt)$");

    if(!list.isEmpty()) {
        for(int i=0;i<list.size();++i) {
            if(exp.indexIn(list.at(i)) == -1) {
                correct = false;
                excessFiles += list.at(i) + "\n";
            } else ++filesCount;
        }

        if(correct) {
            result = true;
        } else {
            result = false;
            cheklabel->setPixmap(QPixmap(":/images/false.png"));
            toolTipText.append(filesCount>0 ? "Лишние файлы в "+excessFiles :
                                              "Отсутствуют требуемые файлы в "+channel+"\n");
            errorLabel->setToolTip(toolTipText);
        }
    } else {
        result = false;
        cheklabel->setPixmap(QPixmap(":/images/false.png"));
        toolTipText.append("Отсутствуют требуемые файлы в "+channel+"\n");
        errorLabel->setToolTip(toolTipText);
    }

    return result;
}

QString FileManager::createDir(QString ratNum, QString ratDesc) {
    QDir ratDir(qApp->applicationDirPath() + "/data/Files/Крыса "+ratNum);
    if(!ratDir.exists()) {
        ratDir.mkdir(qApp->applicationDirPath() + "/data/Files/Крыса "+
                     ratNum);
    }
    QString dirPath = qApp->applicationDirPath() + "/data/Files/Крыса "+
            ratNum + "/" +
            ratDesc;
    QDir dir;
    dir.mkdir(dirPath);

    return dirPath;
}

bool FileManager::zetLabFilesMove(QString dirPath, QString zetLabFilesPath, int ratId) {
    bool result{false};

    QDir zetLabDir(zetLabFilesPath);
    QString newZetLabDirPath = dirPath+"/zetlab";

    if(zetLabDir.rename(zetLabFilesPath, newZetLabDirPath)) {
        result = true;
    }

    dao.insertZetLabFiles(ratId, newZetLabDirPath);

    QString dtuDirPath = dirPath+"/dtu";
    QDir dtuDir;
    dtuDir.mkdir(dtuDirPath);
    dtuDir.setPath(dtuDirPath);

    QDir dir;
    dir.setPath(newZetLabDirPath);
    QStringList list;
    list = dir.entryList(QDir::Files);
    QRegExp exp("*.dtu");
    exp.setPatternSyntax(QRegExp::Wildcard);
    for(int i=0;i<list.size();++i) {
        if(exp.exactMatch(list.at(i))) {
            if(!QFile::rename(newZetLabDirPath+"/"+list.at(i),dtuDirPath+"/"+list.at(i))) result = false;
        }
    }

    list = dtuDir.entryList(QDir::Files);
    dao.insertDtuFiles(ratId,
                       dtuDirPath+"/"+list.at(0),
                       dtuDirPath+"/"+list.at(1),
                       dtuDirPath+"/"+list.at(2),
                       dtuDirPath+"/"+list.at(3),
                       dtuDirPath+"/"+list.at(4),
                       dtuDirPath+"/"+list.at(5));

    return result;
}

bool FileManager::fileMove(int id, QString filePath, QString dirPath, QString type) {
    bool result{false};

    QFile file(filePath);
    if(type == "xml") {
        if(file.rename(dirPath+"/time marks.xml")) {
            dao.insertFile(id, dirPath+"/time marks.xml", type);
            result = true;
            return result;
        }
    }

    if(type == "keyPoints") {
        if(file.rename(dirPath+"/Key Points.txt")) {
            dao.insertFile(id, dirPath+"/Key Points.txt", type);
            result = true;
            return result;
        }
    }

    return result;
}

bool FileManager::resultDirMove(int id, QString resultDirPath, QString dirPath, QString type) {
    bool result{false};

    QDir resultDir(dirPath+"/result");
    if(!resultDir.exists()) {
        resultDir.mkdir(dirPath+"/result");
    }

    resultDir.setPath(resultDirPath);

    if(type == "waveform") {
        if(resultDir.rename(resultDirPath, dirPath+"/result/Waveform")) {
            dao.insertResult(id, dirPath+"/result/Waveform", "waveform");
            result = true;
            return result;
        }
    }

    if(type == "wavelet") {
        if(resultDir.rename(resultDirPath, dirPath+"/result/Wavelet")) {
            dao.insertResult(id, dirPath+"/result/Wavelet", "wavelet");
            result = true;
            return result;
        }
    }

    if(type == "histogram") {
        if(resultDir.rename(resultDirPath, dirPath+"/result/Histogram")) {
            dao.insertResult(id, dirPath+"/result/Histogram", "histogram");
            result = true;
            return result;
        }
    }

    if(type == "spectrum") {
        if(resultDir.rename(resultDirPath, dirPath+"/result/Spectrum")) {
            dao.insertResult(id, dirPath+"/result/Spectrum", "spectrum");
            result = true;
            return result;
        }
    }

    return result;
}

bool FileManager::runCopyTask(QString source, QString dest) {
    copyDir(source, dest);
    emit copyFinished();
}

bool FileManager::copyDir(QString source, QString dest) {
    QDir sourceDir(source), destDir;
    QFileInfoList list = sourceDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for(int i=0;i<list.size();++i) {
        if(list.at(i).isDir()) {
            QString destDir = dest + "/" + list.at(i).fileName();
            QDir dir;
            dir.mkdir(destDir);
            copyDir(list.at(i).path()+"/"+list.at(i).fileName(),destDir);
        }
        if(list.at(i).isFile()) {
            QFile file(list.at(i).filePath());
            if(file.exists()) {
                file.copy(dest+"/"+list.at(i).fileName());
            }
        }
    }
}

//Функция удаления папки; Автор: QtCoder
int removeFolder(QDir & dir) {

    int res = 0;
    //Получаем список каталогов
     QStringList lstDirs = dir.entryList(QDir::Dirs |
                  QDir::AllDirs |
                  QDir::NoDotAndDotDot);
    //Получаем список файлов
     QStringList lstFiles = dir.entryList(QDir::Files);

    //Удаляем файлы
    foreach (QString entry, lstFiles) {
        QString entryAbsPath = dir.absolutePath() + "/" + entry;
        QFile::setPermissions(entryAbsPath, QFile::ReadOwner | QFile::WriteOwner);
        QFile::remove(entryAbsPath);
    }

    //Для папок делаем рекурсивный вызов
     foreach (QString entry, lstDirs) {
         QString entryAbsPath = dir.absolutePath() + "/" + entry;
         QDir dr(entryAbsPath);
         removeFolder(dr);
     }

    //Удаляем обрабатываемую папку
    if (!QDir().rmdir(dir.absolutePath())) {
        res = 1;
    }
    return res;
}


bool FileManager::deleteECGRecord(QString ratNumber, QString ratDesc, int ratId) {
    QDir dir(qApp->applicationDirPath()+"/data/Files/Крыса "+ratNumber+"/"+ratDesc);
    if(dir.exists()) {
        if(removeFolder(dir) == 0) {
            dao.deleteECGRecord(ratId);
            dir.setPath(qApp->applicationDirPath()+"/data/Files/Крыса "+ratNumber);
            if(dir.isEmpty()) {
                removeFolder(dir);
            }
        }
    }
}
