#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include "allresults.h"
#include "ui_allresults.h"

extern QString DATABASE_PATH;

AllResults::AllResults(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AllResults)
{
    ui->setupUi(this);
    dao.connectionToDatabase(qApp->applicationDirPath()+DATABASE_PATH, "allResults");
    ui->RIComboBox->addItem("Все");
    QList< QPair<int, QString> > researchInstitutes = dao.getResearchInstitutes();
    for(int i=0;i<researchInstitutes.count();++i) {
        ui->RIComboBox->addItem(researchInstitutes[i].second);
    }
}

AllResults::~AllResults()
{
    delete ui;
}

void AllResults::recieveResultType(QString type) {
    this->type = type;
}

void AllResults::on_exitButton_clicked()
{
    close();
    delete this;
}

void AllResults::on_showPushButton_clicked()
{
    QList<QString> items;
    if((ui->ratNumberSpinBox->value() == 0) && (ui->RIComboBox->currentText() == "Все")) {
        items = dao.getAllResultType(type);
    } else {
        items = dao.getResultType(type, ui->ratNumberSpinBox->value(), ui->RIComboBox->currentText());
    }
    QWidget *widget = new QWidget(ui->scrollArea);
    QVBoxLayout *widgetLayout = new QVBoxLayout;
    for(int i=0;i<items.count();i+=4) {
        QLabel *DescLabel = new QLabel();
        QString str = "Крыса №:%1(%2)  %3";
        DescLabel->setText(str.arg(items[i])
                              .arg(items[i+1])
                              .arg(items[i+2]));
        widgetLayout->addWidget(DescLabel);

        QLabel *ImgLabel = new QLabel();
        if(type == "histogram") {
            QMovie *movie = new QMovie(items[i+3]);
            ImgLabel->setMovie(movie);
            ImgLabel->setScaledContents(true);
            ImgLabel->setFixedSize(371, 251);
            movie->start();
        } else {
            QPixmap pic(items[i+3]);
            ImgLabel->setPixmap(pic);
            ImgLabel->setScaledContents(true);
            ImgLabel->setFixedSize(371, 251);
        }
        widgetLayout->addWidget(ImgLabel);
    }
    widget->setLayout(widgetLayout);
    ui->scrollArea->setWidget(widget);
}
