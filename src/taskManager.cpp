/***************************************************
* @filename  taskManager.cpp
* @brief
* @author fanjiafei
* @date 2017-11-1
* @version V0.1
****************************************************/

#include "taskManager.h"
#include "unistd.h"
#include "common.h"
#include "Msg2MCUhandler.h"
extern serial* g_serialCom;
extern MsgQueue* g_MsgQueueRecv;
taskManager::taskManager()
{

    if(g_MsgQueueRecv==NULL){
        g_MsgQueueRecv = getMsgQueueRecv();
    }
    if(g_MsgQueueSend==NULL){
        g_MsgQueueSend = getMsgQueueSend();
    }
    if(g_semaphore==NULL){
        g_semaphore = getSemaphore();
    }
}

taskManager::~taskManager()
{

}

/**
 * @brief taskManager::serialComInit
 * @details 初始化串口，设置波特率、停止位、校验位等参数
 */
int taskManager::serialComInit(){
    char ret=-1;
    char portName[]={"/dev/ttyUSB0"};//{"/dev/tty.usbserial"};
    if(g_serialCom==NULL){
        g_serialCom = getSerialCom();
    }
    ret = g_serialCom->Open(portName,115200,8,NO,1);
    if(ret==0)
        UART_Dbg("ERROR open serial com failed");
    else
        UART_Dbg("serialComInit OK\n");
    return ret;
}

/**
 * @brief taskManager::taskRecvSerialMsg
 * @details 1、从串口接收数据
 * 2、对数据进行拼包,将数据入msgQueue
 */
void* taskManager::taskRecvSerialMsg(void *arg){
    int rawLength=-1;
    unsigned char rawData[FramLenMax];
    UART_Dbg("[start] taskRecvSerialMsg\n");
    taskManager* pTaskMngr = (taskManager*)arg;

    while(TRUE){
        rawLength = g_serialCom->Read(rawData);
        if(rawLength>0){
            UART_Dbg("rawData is:");
            for(int i =0;i<rawLength;i++){
                printf("0x%02x ",rawData[i]);
            }
            printf("\n");
            pTaskMngr->mDataLinkLayerProcessor.splitDLFrame(rawData,rawLength);
        }
#ifdef NOTEST
        usleep(5*1000);//ms
#else
        sleep(1);
#endif
    }
    return NULL;
}

/**
 * @brief taskManager::taskProcessSerialMsg
 * @details1、从msgQueue取数据
 * 2、对数据进行COBS解码
 */
void* taskManager::taskProcessSerialMsg(void *arg){
    unsigned char rawData[FramLenMax];
    unsigned char DecodedData[FramLenMax];
    int dataLen;
    int msgQueueLength;
    unsigned char caculateFCS;
    UART_Dbg("[start] taskProcessSerialMsg\n");
    taskManager* pTaskMngr = (taskManager*)arg;

    while(TRUE)
    {
        msgQueueLength = g_MsgQueueRecv->Queuelength();
//        UART_Dbg("[--] taskProcessSerialMsg queueLen=%d\n",msgQueueLength);
        if(msgQueueLength){
            g_MsgQueueRecv->Dequeue(rawData,dataLen);
//            UART_Dbg("RawData is:");for(int i =0;i<dataLen;i++){printf("0x%02x ",rawData[i]);}
            //去除数据首尾的Frame_Header和Frame_Tail，数据形式是:Packet_Header+APP_Data+FCS
            dataLen-=2;//（head tail） 剩余为有效长度
            if(dataLen<2) continue;//防止短数据导致程序异常，如：0x00 0x01 0x00
            pTaskMngr->mCOBStool.UnStuffData(rawData+1,dataLen,DecodedData);
            dataLen--;//COBS源码比转译后编码短1
            UART_Dbg("DecodedData is:");for(int i =0;i<dataLen;i++){printf("0x%02x ",DecodedData[i]);}printf("\n");
            //check CRC
            caculateFCS = GetFCS_8(DecodedData,dataLen-1);
            if(caculateFCS == DecodedData[dataLen-1]){
                UART_Dbg("CRC1:0x%x CRC2:0x%x is correct \n",caculateFCS,DecodedData[dataLen-1]);
            }else{
                UART_Err(" CRC error CRC1:0x%x CRC2:0x%x \n",caculateFCS,DecodedData[dataLen-1]);
                //[todo] add error method
            }
            pTaskMngr->mTransportLayerProcessor.splitTPData(DecodedData,dataLen-1);//TP
        }
#ifdef NOTEST
        usleep(3*1000);//ms
#else
        sleep(1);
#endif

    }
    return NULL;
}
/**
 * @brief taskManager::taskSendSerialMsg
 * @param arg
 * @details 发送数据进程，对数据进行统一发送管理
 * 发送时看该数据是否需要ACK，如果需要ACK则等待ACK，如果等待超时则重发该数据，最多重发两次
 * 发送前判断对方接受区RWS是否还有空间，如果没有空间了则暂时不发送，一直等到RWS不为0，再发送数据
 * @return
 */
void* taskManager::taskSendSerialMsg(void *arg){
    unsigned char rawData[FramLenMax];
    int dataLen;
    int msgQueueLength;
    int ret;
    int clientRWS;
    struct timespec ts;
    bool waitACK=false;
    unsigned sleepTime=100*1000;//ms
    unsigned short resendTimes=0;
    UART_Dbg("[start] taskSendSerialMsg\n");
    UNUSED(arg);

    while(TRUE)
    {
#if 1
        if(waitACK){//等待同步信号
#else
        if(0){//不等待同步信号
#endif
            //设置等待时间
            if (clock_gettime( CLOCK_REALTIME,&ts ) < 0 ){
                usleep(sleepTime);UART_Err("clock_gettime failed\n");continue;
            }
            ts.tv_nsec += nano_sec;ts.tv_sec += ts.tv_nsec/NSECTOSEC;ts.tv_nsec = ts.tv_nsec%NSECTOSEC;
            ret = sem_timedwait(g_semaphore,&ts);
            if (ret == -1) {
                if (errno == ETIMEDOUT&&resendTimes++<2){//超时处理 重发三次
                    UART_Err("sem_timedwait() timed out reSendtimes[%d] clientRWS[%d] Queue[%d]  Data is:",resendTimes,clientRWS,msgQueueLength);for(int i =0;i<dataLen;i++){printf("0x%02x ",rawData[i]);}printf("\n");
                    ret = g_serialCom->Write(rawData,dataLen-1);
                    if(!ret) UART_Err("[taskSendSerialMsg] resend data error!\n");
                }
//                else
//                    perror("sem_timedwait");
                usleep(sleepTime);//ms
                //[todo]重新请求数据
                continue;
            }else{
            //收到同步信号
            UART_Dbg("recv ACK intime\n");
            waitACK=0;//收到ACK后，下个循环不需要继续等待ACK
            resendTimes=0;
            }
        }else{
            usleep(sleepTime);
        }

        //判断对方接受区是否已经满了
        clientRWS=getClientRWS();
        if(clientRWS<1) continue;

        msgQueueLength = g_MsgQueueSend->Queuelength();
//        UART_Dbg("[--] taskSendSerialMsg queueLen=%d\n",msgQueueLength);
        if(msgQueueLength){
            g_MsgQueueSend->Dequeue(rawData,dataLen);
            waitACK=rawData[dataLen-1];//判断发送出去的消息是否需要等待ACK，需要的话则等待
            ret = g_serialCom->Write(rawData,dataLen-1);
            if(!ret) UART_Err("[taskSendSerialMsg] send data error!\n");
            UART_Dbg("clientRWS[%d] Queue[%d] SendData is:",clientRWS,msgQueueLength);for(int i =0;i<dataLen;i++){printf("0x%02x ",rawData[i]);}printf("\n");
        }else{
            waitACK=0;//没有数据发送，不需要等待ACK
        }

    }
    return NULL;
}

/**
 * @brief taskManager::taskSerialTest
 * @param arg
 * @details 测试线程，用于调试时进行数据发送测试
 * @return
 */
void* taskManager::taskSerialTest(void *arg){//用于发送测试命令
     taskManager* pTaskMngr = (taskManager*)arg;
     Msg2MCUhandler* p_Msg2MCUhandler;
     p_Msg2MCUhandler = new Msg2MCUhandler();
     while(1){
        UART_Dbg("\n-------------------------\n[start]sendSingleTLV2MCUtest\n-------------------------\n");
        pTaskMngr->mTransportLayerProcessor.sendSingleTLV2MCUtest();//test sendSingleTLV2MCUtest
        sleep(4);
        UART_Dbg("\n-------------------------\n[start]sendMultiTLV2MCUtest\n-------------------------\n");
        pTaskMngr->mTransportLayerProcessor.sendMultiTLV2MCUtest();//test sendMultiTLV2MCUtest
        sleep(4);
        UART_Dbg("\n-------------------------\n[start]sendDoorInfo\n-------------------------\n");
        p_Msg2MCUhandler->sendDoorInfo(1,2);
        sleep(4);
        UART_Dbg("\n-------------------------\n[start]sendLampInfo\n-------------------------\n");
        p_Msg2MCUhandler->sendLampInfo(1,2);
        sleep(4);
        UART_Dbg("\n-------------------------\n[start]sendKeyInfo\n-------------------------\n");
        p_Msg2MCUhandler->sendKeyInfo(1,2);
        sleep(4);
        UART_Dbg("\n-------------------------\n[start]sendSpeedInfo\n-------------------------\n");
        p_Msg2MCUhandler->sendSpeedInfo(1,2,3);
        sleep(4);
     }
     return NULL;
}
/**
 * @brief taskManager::initSystem
 * @details 初始化系统
 */
void taskManager::initSystem(){
    int ret=-1;
    //初始化串口
    ret = serialComInit();
    while(!ret){
        UART_Err("[Error] serialComInit failed\n");
        sleep(1);
        ret = serialComInit();
    }
    //初始化同步信号量
    ret = sem_init(g_semaphore,0,0);//If pshared has the value 0, then the semaphore is shared between the threads of a process
    if(ret) UART_Err("[Error] sem_init failed\n");



}
/**
 * @brief taskManager::start
 * @details 创建线程
 */
void taskManager::start(){
    int ret=-1;
    initSystem();
    ret = pthread_create(&serialRecv, NULL,taskRecvSerialMsg, this);
    if(ret) UART_Err("[Error] start taskRecvSerialMsg failed\n");
    ret = pthread_create(&serialProcessor, NULL,taskProcessSerialMsg, this);
    if(ret) UART_Err("[Error] start taskProcessSerialMsg failed\n");//若成功则返回0，否则返回出错编号
    ret = pthread_create(&serialSend, NULL,taskSendSerialMsg, this);
    if(ret) UART_Err("[Error] start taskSendSerialMsg failed\n");
//    ret = pthread_create(&serialTest, NULL,taskSerialTest, this);
//    if(ret) UART_Err("[Error] start taskSerialTest failed\n");
//    pthread_join(serialProcessor,NULL);


}

