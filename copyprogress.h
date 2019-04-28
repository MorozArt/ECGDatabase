#ifndef COPYPROGRESS_H
#define COPYPROGRESS_H

#include <QDialog>

namespace Ui {
class CopyProgress;
}

class CopyProgress : public QDialog
{
    Q_OBJECT

public slots:
    void copyFinish();

public:
    explicit CopyProgress(QWidget *parent = nullptr);
    ~CopyProgress();

private slots:
    void on_okButton_clicked();

private:
    Ui::CopyProgress *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // COPYPROGRESS_H
