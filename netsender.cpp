#include "netsender.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "netdb.h"
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "iostream"
#include "string.h"
#include "common.h"
#include "log.h"

NetSender::NetSender(string localIP, string sendIP, int port)
{
    this->port=port;
    this->socketReady=false;
    struct in_addr localInterface;

    /*Create a udp socket ...*/
    this->sockfd= socket(AF_INET,SOCK_DGRAM,0);
    if(this->sockfd<0){
        cout<<"Open socket error.."<<endl;
    }else{
        memset((char *)&groupSock,0,sizeof(groupSock));
        this->groupSock.sin_family=AF_INET;
        this->groupSock.sin_addr.s_addr=inet_addr(sendIP.c_str());
        this->groupSock.sin_port=htons(this->port);
        this->SendSockLen=sizeof(this->groupSock);

        localInterface.s_addr=inet_addr(localIP.c_str());
        if(setsockopt(this->sockfd,IPPROTO_IP,IP_MULTICAST_IF,(char*)&localInterface,sizeof(localInterface))<0){
            Log::RaiseLogInfo(Error,"Socket配置错误!");
        }else{
            this->socketReady=true;
        }
    }

}


bool NetSender::SendTimePacket(TimeInformation *TimeInfo){
    int PACK_SIZE=20;
    char pre_head[4]=PREHEAD;
    TimePacket tp;
    strcpy(tp.Time.Prepacket,pre_head);
    tp.Time.Year=TimeInfo->Year;
    tp.Time.Month=TimeInfo->Month;
    tp.Time.Day=TimeInfo->Day;
    tp.Time.Hour=TimeInfo->Hour;
    tp.Time.Minute=TimeInfo->Minute;
    tp.Time.Second=TimeInfo->Second;
    tp.Time.MSecond=TimeInfo->MSecond;
    tp.Time.Valid_1Hz=(TimeInfo->Valid_1Hz ? VALIDVALUE:INVALIDVALUE);
    tp.Time.Valid_2Hz=(TimeInfo->Valid_2Hz ? VALIDVALUE:INVALIDVALUE);
    tp.Time.Valid_5Hz=(TimeInfo->Valid_5Hz ? VALIDVALUE:INVALIDVALUE);
    tp.Time.Valid_10Hz=(TimeInfo->Valid_10Hz ? VALIDVALUE:INVALIDVALUE);
    tp.Time.Valid_20Hz=(TimeInfo->Valid_20Hz ? VALIDVALUE:INVALIDVALUE);
    tp.Time.Valid_50Hz=(TimeInfo->Valid_50Hz ? VALIDVALUE:INVALIDVALUE);
    if(this->socketReady){
        int sendPackSize=sendto(this->sockfd,tp.Packet,PACK_SIZE,0,(struct sockaddr *) &this->groupSock,this->SendSockLen);
        if(sendPackSize==20)
            return true;
        else
            return false;
    }
    else
        return false;
}

void NetSender::Terminate(){
    close(this->sockfd);
}
