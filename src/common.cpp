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


/**
 * @brief getSerialCom
 * 获取全局的串口指针
 * @return
 */
serial* getSerialCom(){
    if(g_serialCom==NULL){
        g_serialCom = new serial();
    }
    return g_serialCom;
}
/**
 * @brief getMsgQueueRecv
 * 获取全局的数据接收队列
 * @return
 */
MsgQueue* getMsgQueueRecv(){
    if(g_MsgQueueRecv==NULL){
        g_MsgQueueRecv = new MsgQueue(MaxQueueLen);
    }
    return g_MsgQueueRecv;
}
/**
 * @brief getMsgQueueSend
 * 获取全局的数据发送队列
 * @return
 */
MsgQueue* getMsgQueueSend(){
    if(g_MsgQueueSend==NULL){
        g_MsgQueueSend = new MsgQueue(MaxQueueLen);
    }
    return g_MsgQueueSend;
}
/**
 * @brief getSemaphore
 * 获取全局的同步信号量，用于等待ACK
 * @return
 */
sem_t* getSemaphore(){
    if(g_semaphore==NULL){
        g_semaphore = new sem_t();
    }
    return g_semaphore;
}
/**
 * @brief getSN
 * 获取全局的SN号
 * @return
 */
unsigned char getSN(){
    g_SN = g_SN%max_SN;
    return g_SN++;
}

/**
 * @brief getClientRWS
 * 获取客户端的RWS
 * @return
 */
unsigned int getClientRWS(){
    return g_clientRWS;
}

/**
 * @brief setClientRWS
 * 更新客户端的RWS
 * @param rws
 */
void setClientRWS(unsigned int rws){
    g_clientRWS=rws;
}
