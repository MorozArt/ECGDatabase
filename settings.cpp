#include <QFileDialog>
#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
}

Settings::~Settings()
{
    delete ui;
}

/*void Settings::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event);
    delete  this;
}*/

void Settings::on_tempFileButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this);
    ui->tempFileLineEdit->setText(path);
    tempFileCorrect = fileManager.correctFileType(path, "xml", ui->tempFileCheckLabel, ui->label_2);
}
