#include <QFileDialog>
#include "settings.h"
#include "globalvariable.h"
#include "ui_settings.h"
#include "referencemanager.h"
#include "messageboxcreator.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    QSettings settings("database.conf", QSettings::IniFormat);
    dao.connectionToDatabase(qApp->applicationDirPath()+DATABASE_PATH, "settings");

    ui->setupUi(this);
    ui->errorTempFilelabel->setText("");
    ui->tempFileLineEdit->setText(settings.value("TempFilePath", "").toString());
    setResearchInstitutes();
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

void Settings::on_RIDeleteButton_clicked()
{
    if(!ui->RIListWidget->selectedItems().isEmpty()) {
        int index = ui->RIListWidget->currentRow();

        if(MessageBoxCreator::execQuestionMessageBox(this,
               "Вы действительно хотите удалить НИИ "+researchInstitutes.at(index).second+"?") == QMessageBox::Yes) {
            QList<QPair<QString,QString>> records = dao.getAllRecordForResearchInstitute(researchInstitutes.at(index).first);
            if(records.size() == 0) {
                dao.deleteResearchInstitute(researchInstitutes.at(index).first);
                setResearchInstitutes();
            } else {
                QString error = "Невозможно удалить данный НИИ, так как присутствуют записи относящиеся к нему:\n";
                for(int i=0;i<records.size();++i) {
                    error.append("Крыса "+records.at(i).first+" "
                                 +records.at(i).second+"\n");
                }
                MessageBoxCreator::showMessageBox(this, ERROR_TITLE, error);
            }
        }
    }
}

void Settings::setResearchInstitutes() {
    researchInstitutes = dao.getResearchInstitutes();
    ui->RIListWidget->clear();
    for(int i=0; i<researchInstitutes.size();++i) {
        ui->RIListWidget->addItem(researchInstitutes.at(i).second);
    }
}
