#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>
#include "filemanager.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void on_tempFileButton_clicked();

    void on_cancelButton_clicked();

    void on_acceptButton_clicked();

    void on_resetTempFileButton_clicked();

private:
    void closeEvent(QCloseEvent *event);
    Ui::Settings *ui;
    FileManager fileManager;
    bool tempFileCorrect{false};
};

#endif // SETTINGS_H
