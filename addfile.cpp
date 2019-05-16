#include <QFileDialog>
#include "addfile.h"
#include "globalvariable.h"
#include "ui_addfile.h"
#include "messageboxcreator.h"

AddFile::AddFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFile)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);

    ui->errorZetLablabel->setText("");
    ui->errorXMLlabel->setText("");
    ui->errorHistogramLabel->setText("");
    ui->errorWaveformLabel->setText("");
    ui->errorWaveletLabel->setText("");
    ui->errorSpectrumLabel->setText("");
    ui->errorKeyPointsLabel->setText("");
    ui->errorCartLabel->setText("");

    dao.connectionToDatabase(qApp->applicationDirPath()+DATABASE_PATH, "addFile");
    researchInstitutes = dao.getResearchInstitutes();
    for(int i=0;i<researchInstitutes.count();++i) {
        ui->RIComboBox->addItem(researchInstitutes[i].second);
    }
}

AddFile::~AddFile()
{
    delete ui;
    dao.closeConnection(qApp->applicationDirPath()+DATABASE_PATH);
}

void AddFile::on_cancelPushButton_clicked()
{
    close();
}

void AddFile::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event);
    delete  this;
}

void AddFile::on_addPushButton_clicked()
{
    if(zetLabCorrect && !ui->ratDescLineEdit->text().isEmpty()) {
        int ratNumber = ui->ratNumberSpinBox->value();
        QString ratDesc = ui->ratDescLineEdit->text();
        int RIid{-1}, ratId{-1};
        for(int i=0;i<researchInstitutes.count();++i) {
            if(ui->RIComboBox->currentText() == researchInstitutes[i].second) {
                RIid = researchInstitutes[i].first;
                break;
            }
        }
        if( (dao.getRatId(ratNumber, RIid, ratDesc) == -1)) {
            dao.insertECGRecord(ratNumber, ratDesc, RIid);
            ratId = dao.getRatId(ratNumber, RIid, ratDesc);
        } else {
            MessageBoxCreator::showMessageBox(this, ERROR_TITLE, "Такой файл уже существует!");
            return;
        }

        QString dirPath = fileManager.createDir(QString::number(ratNumber), ratDesc);
        int resultId = dao.createResultId(ratId);

        if(!fileManager.zetLabFilesMove(dirPath, ui->zetLabLineEdit->text(), ratId)) {
            qDebug() << "Error in addFile!";
        }

        if(XMLcorrect) {
            fileManager.fileMove(ratId, ui->xmlLineEdit->text(), dirPath, "xml");
        }

        if(waveformCorrect) {
            fileManager.resultDirMove(resultId,ui->waveformLineEdit->text(), dirPath, "waveform");
        }

        if(waveletCorrect) {
            fileManager.resultDirMove(resultId,ui->waveletLineEdit->text(), dirPath, "wavelet");
        }

        if(histogramCorrect) {
            fileManager.resultDirMove(resultId,ui->histogramLineEdit->text(), dirPath, "histogram");
        }

        if(spectrumCorrect) {
            fileManager.resultDirMove(resultId,ui->spectrumLineEdit->text(), dirPath, "spectrum");
        }

        if(keyPointsCorrect) {
            fileManager.fileMove(ratId, ui->keyPointsLineEdit->text(), dirPath, "keyPoints");
        }
    } else {
        if(!zetLabCorrect) {
            MessageBoxCreator::showMessageBox(this, ERROR_TITLE, "Указаны неверные данные ZetLab!");
        }
        if(ui->ratDescLineEdit->text().isEmpty()) {
            MessageBoxCreator::showMessageBox(this, ERROR_TITLE, "Введите описание!");
        }
    }
    close();
}

void AddFile::on_zetLabButton_clicked()
{
    QString str = QFileDialog::getExistingDirectory(this,"Выбор директории","",QFileDialog::ShowDirsOnly);
    ui->zetLabLineEdit->setText(str);
    zetLabCorrect = fileManager.correctZetLabFiles(str, ui->zetLabCheklabel, ui->errorZetLablabel);
}

void AddFile::on_zetLabLineEdit_editingFinished()
{
    if(!ui->zetLabLineEdit->text().isEmpty()) {
        zetLabCorrect = fileManager.correctZetLabFiles(ui->zetLabLineEdit->text(), ui->zetLabCheklabel, ui->errorZetLablabel);
    } else {
        ui->zetLabCheklabel->setPixmap(QPixmap(":/images/false.png"));
        ui->errorZetLablabel->setText("");
        ui->errorZetLablabel->setToolTip("");
    }
}

void AddFile::on_xmlButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this);
    ui->xmlLineEdit->setText(path);
    XMLcorrect = fileManager.correctFileType(path, "xml", ui->XMLChekLabel, ui->errorXMLlabel);
}

void AddFile::on_xmlLineEdit_editingFinished()
{
    if(!ui->xmlLineEdit->text().isEmpty()) {
        XMLcorrect = fileManager.correctFileType(ui->xmlLineEdit->text(), "xml",
                                                 ui->XMLChekLabel, ui->errorXMLlabel);
    } else {
        ui->XMLChekLabel->setPixmap(QPixmap(":/images/NoRequired.png"));
        ui->errorXMLlabel->setText("");
        XMLcorrect = false;
    }
}

void AddFile::on_keyPointsButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this);
    ui->keyPointsLineEdit->setText(path);
    keyPointsCorrect = fileManager.correctFileType(path, "txt", ui->keyPointsChekLabel, ui->errorKeyPointsLabel);
}

void AddFile::on_keyPointsLineEdit_editingFinished()
{
    if(!ui->keyPointsLineEdit->text().isEmpty()) {
        keyPointsCorrect = fileManager.correctFileType(ui->keyPointsLineEdit->text(),
                                                       "txt", ui->keyPointsChekLabel, ui->errorKeyPointsLabel);
    } else {
        ui->keyPointsChekLabel->setPixmap(QPixmap(":/images/NoRequired.png"));
        ui->errorKeyPointsLabel->setText("");
        keyPointsCorrect = false;
    }
}

void AddFile::on_waveformButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Выбор директории","",QFileDialog::ShowDirsOnly);
    ui->waveformLineEdit->setText(path);
    waveformCorrect = fileManager.correctResult(path, "waveform", ui->waveformChekLabel, ui->errorWaveformLabel);
}

void AddFile::on_waveformLineEdit_editingFinished()
{
    if(!ui->waveformLineEdit->text().isEmpty()) {
        waveformCorrect = fileManager.correctResult(ui->waveformLineEdit->text(),
                                                      "waveform", ui->waveformChekLabel, ui->errorWaveformLabel);
    } else {
        ui->waveformChekLabel->setPixmap(QPixmap(":/images/NoRequired.png"));
        ui->errorWaveformLabel->setText("");
        ui->errorWaveformLabel->setToolTip("");
        waveformCorrect = false;
    }
}

void AddFile::on_waveletButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Выбор директории","",QFileDialog::ShowDirsOnly);
    ui->waveletLineEdit->setText(path);
    waveletCorrect = fileManager.correctResult(path, "wavelet", ui->waveletChekLabel, ui->errorWaveletLabel);
}

void AddFile::on_waveletLineEdit_editingFinished()
{
    if(!ui->waveletLineEdit->text().isEmpty()) {
        waveletCorrect = fileManager.correctResult(ui->waveletLineEdit->text(), "wavelet",
                                                        ui->waveletChekLabel, ui->errorWaveletLabel);
    } else {
        ui->waveletChekLabel->setPixmap(QPixmap(":/images/NoRequired.png"));
        ui->errorWaveletLabel->setText("");
        ui->errorWaveletLabel->setToolTip("");
        waveletCorrect = false;
    }
}

void AddFile::on_histogramButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Выбор директории","",QFileDialog::ShowDirsOnly);
    ui->histogramLineEdit->setText(path);
    histogramCorrect = fileManager.correctResult(path, "histogram", ui->histogramChekLabel, ui->errorHistogramLabel);
}

void AddFile::on_histogramLineEdit_editingFinished()
{
    if(!ui->histogramLineEdit->text().isEmpty()) {
        histogramCorrect = fileManager.correctResult(ui->histogramLineEdit->text(), "histogram",
                                                          ui->histogramChekLabel, ui->errorHistogramLabel);
    } else {
        ui->histogramChekLabel->setPixmap(QPixmap(":/images/NoRequired.png"));
        ui->errorHistogramLabel->setText("");
        ui->errorHistogramLabel->setToolTip("");
        histogramCorrect = false;
    }
}

void AddFile::on_spectrumButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Выбор директории","",QFileDialog::ShowDirsOnly);
    ui->spectrumLineEdit->setText(path);
    spectrumCorrect = fileManager.correctResult(path, "spectrum", ui->spectrumChekLabel, ui->errorSpectrumLabel);
}

void AddFile::on_spectrumLineEdit_editingFinished()
{
    if(!ui->spectrumLineEdit->text().isEmpty()) {
        spectrumCorrect = fileManager.correctResult(ui->spectrumLineEdit->text(), "spectrum",
                                                         ui->spectrumChekLabel, ui->errorSpectrumLabel);
    } else {
        ui->spectrumChekLabel->setPixmap(QPixmap(":/images/NoRequired.png"));
        ui->errorSpectrumLabel->setText("");
        ui->errorSpectrumLabel->setToolTip("");
        spectrumCorrect = false;
    }
}
