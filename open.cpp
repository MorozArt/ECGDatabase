#include <QMessageBox>
#include "open.h"
#include "ui_open.h"
#include "messageboxcreator.h"
#include "globalvariable.h"

Open::Open(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::open)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    dao.connectionToDatabase(qApp->applicationDirPath()+DATABASE_PATH, "open");
    ui->RIComboBox->addItem("Все");
    QList< QPair<int, QString> > researchInstitutes = dao.getResearchInstitutes();
    for(int i=0;i<researchInstitutes.count();++i) {
        ui->RIComboBox->addItem(researchInstitutes[i].second);
    }
}

Open::~Open()
{
    delete ui;
    dao.closeConnection(qApp->applicationDirPath()+DATABASE_PATH);
}

void Open::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event);
    delete this;
}

void Open::showInTable(QList<QString> items) {
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    if(items.count() > 0) {
        int row = items.count()/3;
        ui->tableWidget->setRowCount(row);
        row = 0;
        for(int i=0;i<items.count(); i+=3) {
            QTableWidgetItem* itemNumber = new QTableWidgetItem;
            itemNumber->setText(items[i]);
            itemNumber->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, 0, itemNumber);

            QTableWidgetItem* itemRI = new QTableWidgetItem;
            itemRI->setText(items[i+1]);
            itemRI->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, 1, itemRI);

            QTableWidgetItem* itemDesc = new QTableWidgetItem;
            itemDesc->setText(items[i+2]);
            itemDesc->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, 2, itemDesc);
            ++row;
        }
    }
}

void Open::on_showButton_clicked()
{
    showInTable(dao.getOpenRat(ui->spinBox->value(), ui->RIComboBox->currentText()));
}

void Open::on_showAllButton_clicked()
{
    showInTable(dao.getOpenAllRat());
}

void Open::on_deletelButton_clicked()
{
    if(!ui->tableWidget->selectedItems().isEmpty()) {

        if(MessageBoxCreator::execQuestionMessageBox(this,
                          "Вы действительно хотите удалить эту запись?") == QMessageBox::Yes) {

            int ratNumber = ui->tableWidget->selectedItems().first()->text().toInt();
            int RIid = dao.getResearchInstituteFromName(ui->tableWidget->selectedItems().at(1)->text());
            QString ratDesc = ui->tableWidget->selectedItems().last()->text();
            int ratId = dao.getRatId(ratNumber, RIid, ratDesc);
            fileManager.deleteECGRecord(QString::number(ratNumber), ratDesc, ratId);

            showInTable(dao.getOpenRat(ui->spinBox->value(), ui->RIComboBox->currentText()));
        }
    } else {
        MessageBoxCreator::showMessageBoxNoSelectedFiles(this);
    }
}

void Open::on_openButton_clicked()
{
    if(!ui->tableWidget->selectedItems().isEmpty()) {
        int ratNumber = ui->tableWidget->selectedItems().first()->text().toInt();
        int RIid = dao.getResearchInstituteFromName(ui->tableWidget->selectedItems().at(1)->text());
        QString ratDesc = ui->tableWidget->selectedItems().last()->text();
        int ratId = dao.getRatId(ratNumber, RIid, ratDesc);
        emit sendData(ratId);
        close();
    } else {
        MessageBoxCreator::showMessageBoxNoSelectedFiles(this);
    }
}

void Open::on_canselButton_clicked()
{
    close();
}

//--------------------------Разобраться: AppCrash---------------------------------------
/*void open::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    if(!ui->tableWidget->selectedItems().isEmpty()) {
        int ratNumber = ui->tableWidget->selectedItems().first()->text().toInt();
        int RIid = dao.getResearchInstituteFromName(ui->tableWidget->selectedItems().at(1)->text());
        QString ratDesc = ui->tableWidget->selectedItems().last()->text();
        int ratId = dao.getRatId(ratNumber, RIid, ratDesc);
        emit sendData(ratId);
        close();
    } else {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка!");
        msgBox.setWindowIcon(QPixmap(":/images/false.png"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Не выбран ни один файл.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
}*/
