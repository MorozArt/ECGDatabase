#include <QFileDialog>
#include "settings.h"
#include "ui_settings.h"
#include "referencemanager.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    QSettings settings("database.conf", QSettings::IniFormat);

    ui->setupUi(this);
    ui->errorTempFilelabel->setText("");
    ui->tempFileLineEdit->setText(settings.value("TempFilePath", "").toString());
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_cancelButton_clicked()
{
    close();
}

void Settings::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event);
    delete  this;
}

void Settings::on_tempFileButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this);
    ui->tempFileLineEdit->setText(path);
    tempFileCorrect = fileManager.correctFileType(path, "xml", ui->tempFileCheckLabel, ui->errorTempFilelabel);
}

void Settings::on_resetTempFileButton_clicked()
{
    QMessageBox accept(QMessageBox::Question,
                "Внимание!",
                "Вы действительно хотите установить\nзначение по умолчанию?",
                QMessageBox::Yes | QMessageBox::No,
                this);
    accept.setButtonText(QMessageBox::Yes, "Да");
    accept.setButtonText(QMessageBox::No, "Нет");

    if(accept.exec() == QMessageBox::Yes) {
        ui->tempFileLineEdit->setText("../temp.xml");
        tempFileCorrect = true;
        ui->tempFileCheckLabel->setPixmap(QPixmap(":/images/true.png"));
    }
}

void Settings::on_acceptButton_clicked()
{
    QSettings settings("database.conf", QSettings::IniFormat);

    if(tempFileCorrect) {
        settings.setValue("TempFilePath", ui->tempFileLineEdit->text());
        ReferenceManager::setTempFile(ui->tempFileLineEdit->text());
    }

    close();
}
