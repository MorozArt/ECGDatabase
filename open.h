#ifndef OPEN_H
#define OPEN_H

#include <QDialog>
#include "dao.h"
#include "filemanager.h"

namespace Ui {
class open;
}

class Open : public QDialog
{
    Q_OBJECT

public:
    explicit Open(QWidget *parent = nullptr);
    ~Open();

signals:
    void sendData(int ratId);

private slots:
    void on_canselButton_clicked();

    void on_openButton_clicked();

    void on_showAllButton_clicked();

    void on_showButton_clicked();

    //void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_deletelButton_clicked();

private:
    void closeEvent(QCloseEvent *event);
    Ui::open *ui;
    DAO dao;
    FileManager fileManager;
    void showInTable(QList<QString> items);
};

#endif // OPEN_H
