#include "mainwindow.h"
#include "log.h"
#include "ui_mainwindow.h"
#include "configinfo.h"
#include "common.h"
#include "netsender.h"
#include <string>
#include "iostream"
#include "timeadjuster.h"

#include "sys/time.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Button_Stop->setEnabled(false);
    ui->buttonManualAdjust->setEnabled(false);
    ui->text_adjust_intval->setMinimum(1);
    ui->text_adjust_intval->setMaximum(MAXADJUSTINTVAL);
    ui->text_zero_protect->setMinimum(1);
    ui->text_zero_protect->setMaximum(MAXPROTECTRANGE);
    this->logUpdateTimer=new QTimer(this);
    connect(this->logUpdateTimer,SIGNAL(timeout()),this,SLOT(on_logInfoUpdate()));
    this->logUpdateTimer->setInterval(1000);    
    this->logUpdateTimer->start();

    this->updateTimer=new QTimer(this);
    connect(this->updateTimer,SIGNAL(timeout()),this,SLOT(updateInfo()));
    updateTimer->setInterval(1000);

    this->timeAdjuster=new TimeAdjuster;
    this->isServiceAlive=false;
    ConfigInfo * configInfo=new ConfigInfo();
     if(configInfo->LoadConfig(TIMESERVERCONFIGURE)){
         this->autoAdjust=configInfo->isAutoAdjustEnable();
         this->ui->adjust_check->setChecked(this->autoAdjust);
         this->adjustInteval=configInfo->getAdjustInteval();
         this->ui->text_adjust_intval->setValue(this->adjustInteval);
         this->adjustTimer=new QTimer(this);
         connect(this->adjustTimer,SIGNAL(timeout()),this,SLOT(adjustTime()));
         this->zeroProtectRange=configInfo->getZeroProtectRange();
         this->ui->text_zero_protect->setValue(this->zeroProtectRange);
     }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Button_Start_clicked()
{
    this->timeInfo=new TimeInformation{0,0,0,0,0,0,0,0,false,0,false,0,false,0,false,0,false,0,false};
    this->netTimeServer=new TimeService(this->timeInfo);
    this->netTimeServer->start();
    this->isServiceAlive=true;
    this->updateTimer->start();
    this->adjustTimer->setInterval(this->adjustInteval*1000);
    this->adjustTimer->start();
    this->ui->Button_Start->setEnabled(false);
    this->ui->Button_Stop->setEnabled(true);
    this->ui->buttonManualAdjust->setEnabled(true);
}

void MainWindow::on_Button_Stop_clicked()
{
    //stop network time service..
    this->netTimeServer->stop();
    this->isServiceAlive=false;
    this->updateTimer->stop();
    this->adjustTimer->stop();
    this->ui->Button_Start->setEnabled(true);
    this->ui->buttonManualAdjust->setEnabled(false);
}

void MainWindow::updateInfo(){
    QString dateStr=QString::number(this->timeInfo->Year)+"-"+QString::number(this->timeInfo->Month)+"-"+QString::number(this->timeInfo->Day);
    this->ui->dateView->setText(dateStr);
    QString timeStr=QString::number(this->timeInfo->Hour)+":"+QString::number(this->timeInfo->Minute)+":"+QString::number(this->timeInfo->Second)+"."+QString::number(this->timeInfo->MSecond);
    this->ui->time_view->setText(timeStr);
    this->ui->Count1Hz_Text->setText(QString::number(this->timeInfo->Count_1Hz));
    this->ui->Count20Hz_Text->setText(QString::number(this->timeInfo->Count_20Hz));
}

void MainWindow::on_buttonManualAdjust_clicked()
{
    if(this->timeInfo->Year!=0)
        this->timeAdjuster->Adjust(this->timeInfo);
}

void MainWindow::adjustTime(){
    if(this->timeInfo->Year!=0&& this->autoAdjust){
        Log::RaiseLogInfo(Info,"校时操作");
        this->timeAdjuster->Adjust(this->timeInfo,this->zeroProtectRange);
    }
}

void MainWindow::on_text_adjust_intval_valueChanged(int arg1)
{
    this->adjustInteval=arg1;
    if(this->isServiceAlive){
        this->adjustTimer->stop();
        this->adjustTimer->setInterval(arg1*1000);
        this->adjustTimer->start();
    }
}

void MainWindow::on_text_zero_protect_valueChanged(int arg1)
{
    this->zeroProtectRange=arg1;
}

void MainWindow::on_logInfoUpdate(){
    QString logText="";
    LogInfo logInfo=LogInfo{"",Info,""};
    while(Log::GetLogInfo(logInfo)){
        QString logTypeStr="";
        switch (logInfo.infoType) {
        case Info:
            logTypeStr=" [I] ";
            break;
        case Error:
            logTypeStr=" [E] ";
            break;
        case Warning:
            logTypeStr=" [W] ";
            break;
        default:
            break;
        }
        this->ui->logView->append(logInfo.time+logTypeStr+logInfo.infoBody+"\n");
    }

}

void MainWindow::on_adjust_check_stateChanged(int arg1)
{
    if(this->ui->adjust_check->isChecked()){
        this->autoAdjust=true;
    }else{
        this->autoAdjust=false;
    }
}
