#include "common.h"
#include <string>
#include <QQueue>
#include "timegenerater.h"
#include "netsender.h"
#include "pthread.h"
#include "log.h"
using namespace std;


#ifndef TIMESERVICE_H
#define TIMESERVICE_H



class TimeService
{
    pthread_t timeServThread;
    TimeInformation * timeInfo;
    static void * timeServiceThread(void *);
public:
    TimeService(TimeInformation * timeInfo);
    void start();
    void stop();
    string getDateTimeText();
    bool getNetworkStatus();
    bool getDriveSatus();
};

#endif // TIMESERVICE_H
