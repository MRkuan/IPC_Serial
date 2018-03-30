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
 * @details 入队
 * @param buf 待入队数据首地址
 * @param datalength 待入队数据长度
 * @return 0:ok -1:error
 * @retval 0 success
 * @retval -1 failed
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
 * @details 出队
 * @param buf 出队后数据存放位置首地址
 * @param datalength 出队的数据长度
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
/**
 * @brief MsgQueue::Queuelength
 * @details 获取队列长度
 * @return
 */
int MsgQueue::Queuelength(){
    return Qstation.size();
}
/**
 * @brief MsgQueue::getQueueRWS
 * @details 获取队列还能存放多少消息
 * @return
 */
int MsgQueue::getQueueRWS(){
    return maxQueueLength-Qstation.size();
}
