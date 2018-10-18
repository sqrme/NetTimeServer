#include <string>
#include "common.h"
#include <list>
using namespace std;

#ifndef CONFIGINFO_H
#define CONFIGINFO_H


class ConfigInfo
{
    string localIP;

    string sendIP;

    int port;

    bool autoAdjust;

    int adjustInteval;

    int zeroProtectRange;

private:
	  //判断是否为合法ip地址
    bool isIPString(string ipstr);
    //判断是否为数字
    bool isNumber( string numStr);
    //过滤字符空格等字符
    string trimStr(string  str);

public:
    ConfigInfo();

    bool LoadConfig(const string configFileName);

    string getLocalIP();

    string getSendIP();

    int getPort();

    bool isAutoAdjustEnable();

    int getAdjustInteval();

    int getZeroProtectRange();


};

#endif // CONFIGINFO_H
