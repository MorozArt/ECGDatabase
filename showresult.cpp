#include <QDir>
#include <QRegExp>
#include "showresult.h"
#include "ui_showresult.h"
#include "referencemanager.h"
#include "messageboxcreator.h"
#include "globalvariable.h"

ShowResult::ShowResult(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowResult)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
}

ShowResult::~ShowResult()
{
    delete ui;
}

double castSecondsToMinutes(int seconds) {
    double minutes{0}, temp{0};
    temp = seconds % 60;
    temp /= 100;
    minutes = (seconds / 60) + temp;
    return minutes;
}

QString ShowResult::getChannel(int channel) {
    QString result{""};
    switch (channel) {
    case 1:
        result = "НЧ1";
        break;
    case 2:
        result = "НЧ2";
        break;
    case 3:
        result = "НЧ3";
        break;
    case 4:
        result = "ВЧ1";
        break;
    case 5:
        result = "ВЧ2";
        break;
    case 6:
        result = "ВЧ3";
        break;
    }

    return result;
}

void ShowResult::recieveResultType(QString type, int id, int channel) {
    this->type = type;
    this->channel = getChannel(channel);
    if(type == "spectrum") this->setWindowTitle("Спектр "+ this->channel);
    if(type == "wavelet") this->setWindowTitle("Вейвлет "+ this->channel);
    if(type == "histogram") this->setWindowTitle("Гистограмма "+ this->channel);
    if(type == "waveform") this->setWindowTitle("Осциллограмма "+ this->channel);
    if(id >= 0) {
        QString dirPath = (dao.getResult(type, id)+"/"+this->channel);
        QDir dir(dirPath);
        QStringList list;

        {
            QStringList sub = dir.entryList(QDir::Files);
            QRegExp rExp("^("+type+")\\s\\d+\\-\\d+\\.("+IMAGES_FORMAT+")$");
            for(int i=0;i<sub.size();++i) {
                if(rExp.indexIn(sub.at(i)) != -1) list.append(sub.at(i));
            }
        }

        if(!list.isEmpty()) {
            item tempItem;
            for(int i=0; i<list.size();++i) {
                tempItem.pic = dirPath+"/"+list.at(i);

                QString str = list.at(i).split(".").at(0);
                int j = str.size() - 1;
                QChar ch = str[j];
                QString tempStr;
                while (ch != " ") {
                    if (ch != "-") {
                        tempStr.insert(0,ch);
                    } else {
                        tempItem.end = castSecondsToMinutes(tempStr.toInt());
                        tempStr.clear();
                    }
                    --j;
                    ch = str[j];
                }
                tempItem.start = castSecondsToMinutes(tempStr.toInt());
                items.append(tempItem);
            }
            sortItems();

            for(int i=0;i<items.size();++i) {
                QString str = "%1";
                ui->startComboBox->addItem(str.arg(items[i].start,0,'f',2));
                ui->endComboBox->addItem(str.arg(items[i].end,0,'f',2));
            }

        } else {
            MessageBoxCreator::showMessageBox(this, ERROR_TITLE, "Отсутствуют корректные файлы!");
        }
    } else {
        MessageBoxCreator::showMessageBoxNoSelectedFiles(this);
    }
}

void ShowResult::on_closeButton_clicked()
{
    close();
}

void ShowResult::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event);
    delete this;
}

void ShowResult::sortItems() {
    for(int i=0;i<items.size();++i) {
        for (int j=0;j<items.size()-1;++j) {
            if(items[j].start > items[j+1].start) items.swap(j,j+1);
        }
    }
}

void ShowResult::on_showButton_clicked()
{
    double startValue = ui->startComboBox->currentText().toDouble(),
        endValue = ui->endComboBox->currentText().toDouble();
    if((startValue < endValue) && (startValue != endValue)) {
        checkedItems.clear();
        QWidget *widget = new QWidget(ui->scrollArea);
        QVBoxLayout *widgetLayout = new QVBoxLayout;
        for (int i=0; i<items.size();++i) {
            QLabel *descLabel = new QLabel();
            QLabel *imgLabel = new QLabel();
            QCheckBox *checkBox = new QCheckBox();

            if(items[i].start >= startValue && items[i].end <= endValue) {
                QString str = "с %1 по %2:";
                descLabel->setText(str.arg(items[i].start,0,'f',2)
                                      .arg(items[i].end,0,'f',2));
                widgetLayout->addWidget(descLabel);
                QHBoxLayout *checkLayout = new QHBoxLayout;
                QLabel *checkLabel = new QLabel();
                checkLabel->setText("Выбрать для получения");
                checkLayout->addWidget(checkLabel);
                checkLayout->addWidget(checkBox);
                QLabel *stubLabel = new QLabel();
                stubLabel->setText("              ");
                checkLayout->addWidget(stubLabel);
                widgetLayout->addLayout(checkLayout);

                imgLabel->setPixmap(QPixmap(items[i].pic));
                imgLabel->setScaledContents(true);
                imgLabel->setFixedSize(371, 251);
                widgetLayout->addWidget(imgLabel);

                QPair<QCheckBox*, QString> checkItem;
                checkItem.first = checkBox;
                checkItem.second = items[i].pic;
                checkedItems.append(checkItem);
            }
        }
        widget->setLayout(widgetLayout);
        ui->scrollArea->setWidget(widget);
    } else {
        MessageBoxCreator::showMessageBox(this, ERROR_TITLE, "Указан неверный диапазон!");
    }
}

void ShowResult::on_getButton_clicked()
{
    int count{0};
    bool missingFiles{false};
    QList<QString> missingFilesList;
    QList<QPair<QString, QString> > list;
    QPair<QString, QString> pair;
    for(int i=0;i<checkedItems.size();++i) {
        if(checkedItems.at(i).first->isChecked()) {
            QString str = checkedItems.at(i).second;
            str.replace(QRegularExpression("\\.("+IMAGES_FORMAT+")"), ".txt");
            if(QFile::exists(str)) {
                pair.first = type;
                pair.second = str;
                list.append(pair);
            } else {
                missingFiles = true;
                missingFilesList.append(str);
            }
        } else {
            ++count;
        }
    }

    if(count == checkedItems.size()) {
        MessageBoxCreator::showMessageBoxNoSelectedFiles(this);
    } else {
        if(list.size() > 0) {
            ReferenceManager::writeRef(list);
            if(missingFiles) {
                QString errorStr{""};
                for(int i=0;i<missingFilesList.size();++i) {
                    errorStr.append(QStringList(missingFilesList.at(i).split("/")).last()+"\n");
                }
                MessageBoxCreator::showMessageBox(this, ERROR_TITLE, "Отсутствуют следующие файлы:\n"+errorStr);
            }
            MessageBoxCreator::showMessageBox(this, READY_TITLE, "Полученны ссылки на существующие файлы");
        } else {
            MessageBoxCreator::showMessageBox(this, ERROR_TITLE, "Файлы не найдены!");
        }
    }
}
