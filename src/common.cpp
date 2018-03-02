#include "common.h"

/**
 * @brief GetFCS_8
 * @param pbuf
 * @param len
 * @return
 * 获取一个1B的FCS校验和
 */
unsigned char GetFCS_8(unsigned char *pbuf, unsigned int len)
{
    unsigned int i;
    unsigned char tmpFCS = 0;

    for (i = 0; i < len; i++)
    {
        tmpFCS ^= pbuf[i];
    }
    return tmpFCS;
}



serial* getSerialCom(){
    if(g_serialCom==NULL){
        g_serialCom = new serial();
    }
    return g_serialCom;
}

MsgQueue* getMsgQueueRecv(){
    if(g_MsgQueueRecv==NULL){
        g_MsgQueueRecv = new MsgQueue(MaxQueueLen);
    }
    return g_MsgQueueRecv;
}
MsgQueue* getMsgQueueSend(){
    if(g_MsgQueueSend==NULL){
        g_MsgQueueSend = new MsgQueue(MaxQueueLen);
    }
    return g_MsgQueueSend;
}
sem_t* getSemaphore(){
    if(g_semaphore==NULL){
        g_semaphore = new sem_t();
    }
    return g_semaphore;
}

unsigned char getSN(){
    g_SN = g_SN%max_SN;
    return g_SN++;
}
unsigned int getClientRWS(){
    return g_clientRWS;
}
void setClientRWS(unsigned int rws){
    g_clientRWS=rws;
}
