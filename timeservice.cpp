#include "timeservice.h"
#include "timegenerater.h"
#include "configinfo.h"
#include "netsender.h"
#include "iostream"
#include "unistd.h"
#include "pthread.h"
#include "log.h"
using namespace std;


TimeService::TimeService(TimeInformation * timeInfo)
{
    this->timeInfo=timeInfo;
}

void * TimeService::timeServiceThread(void *args){
    int res=pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    if(res!=0){
        Log::RaiseLogInfo(Error,"线程可取消属性配置错误。");
        exit(EXIT_FAILURE);
    }

    res=pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
    if(res!=0){
        Log::RaiseLogInfo(Error,"线程可取消类型配置错误!");
        exit(EXIT_FAILURE);
    }

    ConfigInfo * configInfo=new ConfigInfo();
    if(configInfo->LoadConfig(TIMESERVERCONFIGURE)){
        string localIP=configInfo->getLocalIP();
        string sendIP=configInfo->getSendIP();
        int port=configInfo->getPort();
        NetSender* netSender=new NetSender(localIP,sendIP,port);
        TimeInformation * timeInfo=(TimeInformation*) args;
        TimeGenerater* timeGenerater=new TimeGenerater(timeInfo);
        Log::RaiseLogInfo(Info,"时间服务启动。");
        while(true){
            if(timeGenerater->UpdateTime()){
                netSender->SendTimePacket(timeInfo);
            }
            else{
                Log::RaiseLogInfo(Error,"获取时间信息错误！");
            }
        }
    }
    else{
        //cout<<"Configure file is loaded with error!"<<endl;
    }
}


void TimeService::start(){
    int res=pthread_create(&this->timeServThread,NULL,timeServiceThread,this->timeInfo);
    if(res!=0){
        Log::RaiseLogInfo(Error,"线程创建失败！");
        //return false;
    }
}

void TimeService::stop(){
    //cancel thread..
    int res=pthread_cancel(this->timeServThread);
    if(res!=0){
        Log::RaiseLogInfo(Error,"线程终止失败！");
    }
}
