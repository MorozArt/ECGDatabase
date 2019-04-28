#ifndef ALLRESULTS_H
#define ALLRESULTS_H

#include <QDialog>
#include "dao.h"

namespace Ui {
class AllResults;
}

class AllResults : public QDialog
{
    Q_OBJECT

public:
    explicit AllResults(QWidget *parent = nullptr);
    ~AllResults();

public slots:
    void recieveResultType(QString type);

private slots:
    void on_exitButton_clicked();

    void on_showPushButton_clicked();

private:
    Ui::AllResults *ui;
    DAO dao;
    QString type;
};

#endif // ALLRESULTS_H
