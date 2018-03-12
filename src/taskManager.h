/***************************************************
* @filename  taskManager.h
* @brief 串口消息处理类
* @author fanjiafei
* @date 2017-11-1
* @version V0.1
****************************************************/

#ifndef taskManager_H
#define taskManager_H
#include "common.h"
#include "MsgQueue.h"
#include "TLVtools.h"
#include "DataLinkLayer.h"
#include "TransportLayer.h"
#include <pthread.h>
#include "SerialServer.h"
class taskManager
{
public:
    taskManager();
    ~taskManager();
    int serialComInit();
    static void* taskRecvSerialMsg(void *);
    static void* taskProcessSerialMsg(void *);
    static void* taskSendSerialMsg(void *);
    static void* taskSerialTest(void *);
    void start();
    void initSystem();

private:


    pthread_t serialRecv;
    pthread_t serialProcessor;
    pthread_t serialSend;
    pthread_t serialTest;
    DataLinkLayer mDataLinkLayerProcessor;
    TransportLayer mTransportLayerProcessor;
    COBSTool mCOBStool;

};

#endif // taskManager_H
