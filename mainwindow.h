#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "timeservice.h"
#include "common.h"
#include "netsender.h"
#include "timeadjuster.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Button_Start_clicked();

    void on_Button_Stop_clicked();

    void updateInfo();

    void adjustTime();

    void on_buttonManualAdjust_clicked();

    void on_text_adjust_intval_valueChanged(int arg1);

    void on_text_zero_protect_valueChanged(int arg1);

    void on_logInfoUpdate();

    void on_adjust_check_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    TimeService* netTimeServer;
    TimeInformation *timeInfo;
    NetSender *netSender;
    TimeAdjuster *timeAdjuster;
    QTimer * updateTimer;
    QTimer * adjustTimer;
    QTimer * logUpdateTimer;
    bool autoAdjust;
    int adjustInteval;
    int zeroProtectRange;
    bool isServiceAlive;
};

#endif // MAINWINDOW_H
