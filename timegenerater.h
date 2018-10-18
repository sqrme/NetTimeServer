#include "common.h"
#ifndef TIMEGENERATER_H
#define TIMEGENERATER_H


class TimeGenerater
{
    TimeInformation * timeInfo;
public:
    TimeGenerater(TimeInformation *timeInfo);
    bool UpdateTime();
};

#endif // TIMEGENERATER_H
