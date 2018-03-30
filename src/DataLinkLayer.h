/**

* @file       DataLinkLayer.h

* @brief      数据链路层

* @details 处理数据链路层的业务.

* @author     Albert

*/
#ifndef DATALINKLAYER_H
#define DATALINKLAYER_H
#include "MsgQueue.h"


class DataLinkLayer
{
public:
    DataLinkLayer();
    ~DataLinkLayer();

    void splitDLFrame(unsigned char* buf,int datalength);


};

#endif // DATALINKLAYER_H
