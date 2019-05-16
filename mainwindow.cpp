#include <QtConcurrent/QtConcurrent>
#include <QMovie>
#include "mainwindow.h"
#include "globalvariable.h"
#include "ui_mainwindow.h"
#include "open.h"
#include "receivegraphictask.h"
#include "allresults.h"
#include "showresult.h"
#include "addfile.h"
#include "filemanager.h"
#include "copyprogress.h"
#include "messageboxcreator.h"
#include "settings.h"

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);
    if(!dao.connectionToDatabase(qApp->applicationDirPath()+DATABASE_PATH)) {
        qDebug() << "No connection!";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_action_triggered()
{
    Open *op = new Open(this);
    connect(op, SIGNAL(sendData(int)), this, SLOT(recieveOpenData(int)));
    op->exec();
}

void MainWindow::on_add_action_triggered()
{
    AddFile *af = new AddFile(this);
    af->exec();
}

void MainWindow::on_settings_action_triggered()
{
    Settings *settings = new Settings(this);
    settings->exec();
}

void MainWindow::on_ZEtLab_action_triggered()
{
    if(currentSignal.getRatId() > 0) {
        QList<QPair<QString, QString> > list = dao.getZetLabRef(getChannel(), currentSignal.getRatId());
        QString channelType{""};
        switch (getChannel()) {
        case 1:
            channelType = "НЧ1";
            break;
        case 2:
            channelType = "НЧ2";
            break;
        case 3:
            channelType = "НЧ3";
            break;
        case 4:
            channelType = "ВЧ1";
            break;
        case 5:
            channelType = "ВЧ2";
            break;
        case 6:
            channelType = "ВЧ3";
            break;
        }
        if(ReferenceManager::writeRef(list)) {
            MessageBoxCreator::showMessageBox(this, READY_TITLE, "Получены ссылки на файлы ZetLab для "+channelType);
        } else {
            MessageBoxCreator::showMessageBox(this, ERROR_TITLE, "Не удалось получить ссылки на требуемые файлы!");
        }
    } else {
        MessageBoxCreator::showMessageBoxNoSelectedFiles(this);
    }
}

void MainWindow::on_getAll_action_triggered()
{
    if(currentSignal.getRatId() != -1) {
        QString destPath = QFileDialog::getExistingDirectory(this,"Выбор директории","",QFileDialog::ShowDirsOnly);
        if(destPath == "") {
            return;
        }
        destPath += ("/Крыса "+QString::number(currentSignal.getRatNumber()) + " " + currentSignal.getRatDescription());
        QString sourceDir(qApp->applicationDirPath() + "/data/Files/Крыса " +
                 QString::number(currentSignal.getRatNumber()) + "/" + currentSignal.getRatDescription());
        QDir dir;
        if(!dir.mkdir(destPath)) {
            MessageBoxCreator::showMessageBox(this, ERROR_TITLE,
                   "Не удалось создать директорию!\n(Возможно директория с таким названием уже существует)");
            return;
        }

        FileManager *fileManager = new FileManager();
        CopyProgress *prog = new CopyProgress(this);
        connect(fileManager, SIGNAL(copyFinished()), prog, SLOT(copyFinish()));
        prog->show();
        QtConcurrent::run(fileManager, &FileManager::runCopyTask,sourceDir, destPath);

    } else {
        MessageBoxCreator::showMessageBoxNoSelectedFiles(this);
    }
}

void MainWindow::recieveOpenData(int ratId) {
    currentSignal = dao.getSignal(ratId);
    ui->ratNumberLineEdit->setText(QString::number(currentSignal.getRatNumber()));
    ui->ratDescLineEdit->setText(currentSignal.getRatDescription());
    ui->ratRILineEdit->setText(currentSignal.getRatRI());
}

void MainWindow::recieveGraphicData(QLineSeries *series) {
    QList<QLineSeries*> timeMarks;
    ui->graphicsView->setTimeMarks(currentSignal.getCallouts());
    QLineSeries *timeMark;
    for(int i=0;i<currentSignal.getCallouts().count();++i) {
        timeMark = new QLineSeries();
        timeMark->append(currentSignal.getCallouts()[i].first,1);
        timeMark->append(currentSignal.getCallouts()[i].first,-1);
        connect(timeMark, &QLineSeries::hovered, ui->graphicsView, &ECGChartView::tooltip);
        timeMarks.append(timeMark);
    }

    QLineSeries *timeAxis = new QLineSeries(), *globalTimeAxis = new QLineSeries();
    double maxTime = (series->at(series->count()-1).x()+0.001)/60;
    int intMaxTime = static_cast<int>(maxTime);
    for(int i=0;i<=intMaxTime;++i) {
        timeAxis->append(i,0);
        globalTimeAxis->append(i+globalTimeStart,0);
    }
    if(maxTime-intMaxTime > 0.01) {
        timeAxis->append(maxTime,0);
        globalTimeAxis->append(maxTime+globalTimeStart,0);
    }

    graphicsDrawer.drowGraphic(ui->graphicsView, series, timeAxis, globalTimeAxis, timeMarks);
    graphicButtonUnlock();
}

void MainWindow::on_LF1Button_clicked()
{
    ui->LF1RadioButton->setChecked(true);
    startReceiveGraphicTask("LF1");
}

void MainWindow::on_LF2Button_clicked()
{
    ui->LF2RadioButton->setChecked(true);
    startReceiveGraphicTask("LF2");
}

void MainWindow::on_LF3Button_clicked()
{
    ui->LF3RadioButton->setChecked(true);
    startReceiveGraphicTask("LF3");
}

void MainWindow::on_HF1Button_clicked()
{
    ui->HF1RadioButton->setChecked(true);
    startReceiveGraphicTask("HF1");
}

void MainWindow::on_HF2Button_clicked()
{
    ui->HF2RadioButton->setChecked(true);
    startReceiveGraphicTask("HF2");
}

void MainWindow::on_HF3Button_clicked()
{
    ui->HF3RadioButton->setChecked(true);
    startReceiveGraphicTask("HF3");
}

void MainWindow::graphicButtonLock() {
    ui->HF1Button->setEnabled(false);
    ui->HF2Button->setEnabled(false);
    ui->HF3Button->setEnabled(false);
    ui->LF1Button->setEnabled(false);
    ui->LF2Button->setEnabled(false);
    ui->LF3Button->setEnabled(false);
}

void MainWindow::graphicButtonUnlock() {
    ui->HF1Button->setEnabled(true);
    ui->HF2Button->setEnabled(true);
    ui->HF3Button->setEnabled(true);
    ui->LF1Button->setEnabled(true);
    ui->LF2Button->setEnabled(true);
    ui->LF3Button->setEnabled(true);
}

void MainWindow::startReceiveGraphicTask(QString type) {
    if(currentSignal.getDtuId() >= 0) {
        ReceiveGraphicTask* task = new ReceiveGraphicTask(currentSignal.getDtuId(), type);

        connect(task, SIGNAL(send(QLineSeries*)), this, SLOT(recieveGraphicData(QLineSeries*)));
        connect(task,SIGNAL(finished()), task,SLOT(deleteLater()));
        QtConcurrent::run(task,&ReceiveGraphicTask::doWork);
        graphicButtonLock();
    } else {
        MessageBoxCreator::showMessageBoxNoSelectedFiles(this);
    }
}

void MainWindow::on_zoomInButton_clicked()
{
    ui->graphicsView->chart()->zoom(1+zoomValue);
}

void MainWindow::on_zoomOutButton_clicked()
{
    ui->graphicsView->chart()->zoom(1-zoomValue);
}

void MainWindow::on_scrollRightButton_clicked()
{
    ui->graphicsView->chart()->scroll(scrollValue,0);
}

void MainWindow::on_scrollLeftButton_clicked()
{
    ui->graphicsView->chart()->scroll(-scrollValue,0);
}

void MainWindow::on_scrollUpButton_clicked()
{
    ui->graphicsView->chart()->scroll(0,scrollValue);
}

void MainWindow::on_scrollDawnButton_clicked()
{
    ui->graphicsView->chart()->scroll(0,-scrollValue);
}

void MainWindow::on_scaleInButton_clicked()
{
    if(graphicsDrawer.getChart() != nullptr) {
        axisXRange += axisXRangeScale;
        axisXTimeRange += axisXTimeRangeScale;
        graphicsDrawer.getChart()->axes(Qt::Horizontal).first()->setRange(0, axisXRange);
        graphicsDrawer.getChart()->axes(Qt::Horizontal).at(1)->setRange(0, axisXTimeRange);
        graphicsDrawer.getChart()->axes(Qt::Horizontal).last()->setRange(globalTimeStart, axisXTimeRange+globalTimeStart);
    }
}

void MainWindow::on_scaleOutButton_clicked()
{
    if(axisXRange > (axisXRangeScale+0.0001) && graphicsDrawer.getChart() != nullptr) {
        axisXRange -= axisXRangeScale;
        axisXTimeRange -= axisXTimeRangeScale;
        graphicsDrawer.getChart()->axes(Qt::Horizontal).first()->setRange(0, axisXRange);
        graphicsDrawer.getChart()->axes(Qt::Horizontal).at(1)->setRange(0, axisXTimeRange);
        graphicsDrawer.getChart()->axes(Qt::Horizontal).last()->setRange(globalTimeStart, axisXTimeRange+globalTimeStart);
    }
}

void MainWindow::on_waveformButton_clicked()
{
    if(currentSignal.getRatId() != -1) {
        showResult("waveform");
    } else {
        MessageBoxCreator::showMessageBoxNoSelectedFiles(this);
    }
}

void MainWindow::on_waveletButton_clicked()
{
    if(currentSignal.getRatId() != -1) {
        showResult("wavelet");
    } else {
        MessageBoxCreator::showMessageBoxNoSelectedFiles(this);
    }
}

void MainWindow::on_histogrammButton_clicked()
{
    if(currentSignal.getRatId() != -1) {
        showResult("histogram");
    } else {
        MessageBoxCreator::showMessageBoxNoSelectedFiles(this);
    }
}

void MainWindow::on_spectrumButton_clicked()
{
    if(currentSignal.getRatId() != -1) {
        showResult("spectrum");
    } else {
        MessageBoxCreator::showMessageBoxNoSelectedFiles(this);
    }
}

void MainWindow::showResult(QString type) {
    ShowResult *showResultView = new ShowResult(this);
    connect(this, SIGNAL(sendResultType(QString, int, int)), showResultView, SLOT(recieveResultType(QString, int, int)));
    emit sendResultType(type, currentSignal.getResultId(), getChannel());
    showResultView->exec();
}

int MainWindow::getChannel() {
    if(ui->LF1RadioButton->isChecked()) return 1;
    if(ui->LF2RadioButton->isChecked()) return 2;
    if(ui->LF3RadioButton->isChecked()) return 3;
    if(ui->HF1RadioButton->isChecked()) return 4;
    if(ui->HF2RadioButton->isChecked()) return 5;
    if(ui->HF3RadioButton->isChecked()) return 6;
    return -1;
}
