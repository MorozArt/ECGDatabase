#ifndef SHOWRESULT_H
#define SHOWRESULT_H

#include <QDialog>
#include "dao.h"

struct item {
    QString pic;
    double start;
    double end;
};

namespace Ui {
class ShowResult;
}

class ShowResult : public QDialog
{
    Q_OBJECT

public:
    explicit ShowResult(QWidget *parent = nullptr);
    ~ShowResult();

public slots:
    void recieveResultType(QString type, int id, int channel);

private slots:
    void on_closeButton_clicked();

    void on_showButton_clicked();

    void on_getButton_clicked();

private:
    void closeEvent(QCloseEvent *event);
    void sortItems();
    QString getChannel(int channel);
    Ui::ShowResult *ui;
    DAO dao;
    QString type;
    QString channel;
    QList<item> items;
    QList< QPair<QCheckBox*, QString> > checkedItems;
};

#endif // SHOWRESULT_H
