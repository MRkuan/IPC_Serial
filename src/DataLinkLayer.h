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
