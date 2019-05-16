#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include "dao.h"
#include "currentsignal.h"
#include "ecggraphicdrawer.h"
#include "referencemanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    //void sendResultType(QString type);
    void sendResultType(QString type, int id, int channel);

public slots:
    void recieveOpenData(int ratId);
    void recieveGraphicData(QLineSeries *series);

private slots:
    void on_open_action_triggered();

    void on_LF1Button_clicked();

    void on_zoomInButton_clicked();

    void on_zoomOutButton_clicked();

    void on_scrollRightButton_clicked();

    void on_scrollLeftButton_clicked();

    void on_scrollUpButton_clicked();

    void on_scrollDawnButton_clicked();

    void on_scaleInButton_clicked();

    void on_scaleOutButton_clicked();

    void on_LF2Button_clicked();

    void on_waveformButton_clicked();

    void on_histogrammButton_clicked();

    void on_waveletButton_clicked();

    void on_spectrumButton_clicked();

    void on_add_action_triggered();

    void on_LF3Button_clicked();

    void on_HF1Button_clicked();

    void on_HF2Button_clicked();

    void on_HF3Button_clicked();

    void on_ZEtLab_action_triggered();

    void on_getAll_action_triggered();

    void on_settings_action_triggered();

private:
    void graphicButtonLock();
    void graphicButtonUnlock();
    void startReceiveGraphicTask(QString type);
    void resultImageView(QString type);
    void showResult(QString type);
    int getChannel();
    Ui::MainWindow *ui;
    DAO dao;
    ECGGraphicDrawer graphicsDrawer;
    CurrentSignal currentSignal;
};

#endif // MAINWINDOW_H
