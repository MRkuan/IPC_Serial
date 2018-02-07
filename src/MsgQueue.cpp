/***************************************************
* @filename msgqueue.cpp
* @brief
* @author fanjiafei
* @date 2017-11-1
* @version V0.1
****************************************************/

#include "MsgQueue.h"
#include "common.h"
MsgQueue::MsgQueue()
{

}

MsgQueue::~MsgQueue()
{

}
MsgQueue::MsgQueue(unsigned int m_maxQueueLen){
    maxQueueLength = m_maxQueueLen;
}

/**
 * @brief MsgQueue::Enqueue
 * @param buf
 * @param datalength
 * @return 0:ok -1:error
 */
int MsgQueue::Enqueue(unsigned char* buf,int datalength){
    if(Qstation.size()>=maxQueueLength) {
        UART_Err("Queue is full\n");
        return -1;
    }
    Queue_Station * pQueue=new(Queue_Station);
    pQueue->basedata=(unsigned char*)malloc(FramLenMax);
    pQueue->basedatalength=datalength;
    memcpy(pQueue->basedata,buf,datalength);
    Qstation.push(pQueue);
    return 0;
}

/**
 * @brief MsgQueue::Dequeue
 * @param buf
 * @param datalength
 * @return
 */
int MsgQueue::Dequeue(unsigned char* buf,int& datalength){

    if(Qstation.empty()) return -1;
    Queue_Station * pQueue = Qstation.front();
    memcpy(buf,pQueue->basedata,pQueue->basedatalength);
    datalength=pQueue->basedatalength;
    free(pQueue->basedata);
    delete pQueue;
    Qstation.pop();
    return 0;
}

int MsgQueue::Queuelength(){
    return Qstation.size();
}

int MsgQueue::getQueueRWS(){
    return maxQueueLength-Qstation.size();
}
