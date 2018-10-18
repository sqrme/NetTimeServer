#include "common.h"
#include <string>
#include "sys/socket.h"
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include "netinet/in.h"
#include "netdb.h"
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
using namespace std;

#ifndef NETSENDER_H
#define NETSENDER_H



class NetSender
{
    int port;
    bool socketReady;
    struct sockaddr_in groupSock;
    int sockfd;
    int SendSockLen;
public:
    NetSender(string localIP, string sendIP, int port );
    bool SendTimePacket(TimeInformation *TimeInfo);
    void Terminate();
};

#endif // NETSENDER_H
