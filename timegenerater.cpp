#include "timegenerater.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "iostream"
#include "sys/time.h"
using namespace std;

TimeGenerater::TimeGenerater(TimeInformation *timeInfo)
{
    this->timeInfo=timeInfo;
}

bool TimeGenerater::UpdateTime(){
    struct tm * tm_ptr;
    time_t now;
    struct timeval tv;
    int msec=-1;
    this->timeInfo->Valid_10Hz=false;
    this->timeInfo->Valid_1Hz=false;
    this->timeInfo->Valid_20Hz=false;
    this->timeInfo->Valid_2Hz=false;
    this->timeInfo->Valid_50Hz=false;
    this->timeInfo->Valid_5Hz=false;
    time(&now);
    while (true) {
        tm_ptr=localtime(&now);
        //get precise time with useconds

        gettimeofday(&tv,NULL);
        msec=(int)(tv.tv_usec/1000);
        if(tv.tv_usec %10000==0){
            this->timeInfo->Year=tm_ptr->tm_year+1900;
            this->timeInfo->Month=tm_ptr->tm_mon+1;
            this->timeInfo->Day=tm_ptr->tm_mday;
            this->timeInfo->Hour=tm_ptr->tm_hour;
            this->timeInfo->Minute=tm_ptr->tm_min;
            this->timeInfo->Second=tm_ptr->tm_sec;
            this->timeInfo->MSecond=msec;
            //50Hz
            if((tv.tv_usec %20000)==0){
                this->timeInfo->Valid_50Hz=true;
                this->timeInfo->Count_50Hz++;
            }
            //20Hz
            if((tv.tv_usec %50000)==0){
                this->timeInfo->Valid_20Hz=true;
                this->timeInfo->Count_20Hz++;
            }
            //10Hz
            if((tv.tv_usec %100000)==0){
                this->timeInfo->Valid_10Hz=true;
                this->timeInfo->Count_10Hz++;
            }
            //5Hz
            if((tv.tv_usec %200000)==0){
                this->timeInfo->Valid_5Hz=true;
                this->timeInfo->Count_5Hz++;
            }
            //2Hz
            if((tv.tv_usec %500000)==0){
                this->timeInfo->Valid_2Hz=true;
                this->timeInfo->Count_2Hz++;
            }
            //1Hz
            if((tv.tv_usec%1000000==0)){
                this->timeInfo->Valid_1Hz=true;
                this->timeInfo->Count_1Hz++;
            }
            break;
        }
        //sleep
    }
    return true;
}
