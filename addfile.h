#ifndef ADDFILE_H
#define ADDFILE_H

#include <QDialog>
#include "dao.h"
#include "filemanager.h"

namespace Ui {
class AddFile;
}

class AddFile : public QDialog
{
    Q_OBJECT

public:
    explicit AddFile(QWidget *parent = nullptr);
    ~AddFile();

private slots:
    void on_cancelPushButton_clicked();

    void on_addPushButton_clicked();

    void on_zetLabButton_clicked();

    void on_zetLabLineEdit_editingFinished();

    void on_xmlButton_clicked();

    void on_xmlLineEdit_editingFinished();

    void on_waveletButton_clicked();

    void on_waveletLineEdit_editingFinished();

    void on_histogramButton_clicked();

    void on_histogramLineEdit_editingFinished();

    void on_spectrumButton_clicked();

    void on_spectrumLineEdit_editingFinished();

    void on_keyPointsButton_clicked();

    void on_keyPointsLineEdit_editingFinished();

    void on_waveformButton_clicked();

    void on_waveformLineEdit_editingFinished();

private:
    void closeEvent(QCloseEvent *event);
    Ui::AddFile *ui;
    DAO dao;
    FileManager fileManager;
    QList< QPair<int, QString> > researchInstitutes;
    bool zetLabCorrect{false}, XMLcorrect{false}, waveformCorrect{false}, waveletCorrect{false},
    histogramCorrect{false}, spectrumCorrect{false}, keyPointsCorrect{false};
};

#endif // ADDFILE_H
