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
}

taskManager::~taskManager()
{

}

/**
 * @brief taskManager::serialComInit
 * 初始化串口，设置波特率、停止位、校验位等参数
 */
int taskManager::serialComInit(){
    char ret=-1;
    char portName[]={"/dev/tty.usbserial"};
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
 * 1、从串口接收数据
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
        usleep(50*1000);//50ms
#else
        sleep(5);
#endif
    }

}

/**
 * @brief taskManager::taskProcessSerialMsg
 * 1、从msgQueue取数据
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
            //[todo] analyse and solve data
            pTaskMngr->mTransportLayerProcessor.splitTPData(DecodedData,dataLen-1);//TP
        }
//        UART_Dbg("[start]sendSingleTLV2MCUtest\n");
        pTaskMngr->mTransportLayerProcessor.sendSingleTLV2MCUtest();//test sendSingleTLV2MCUtest

//        UART_Dbg("[start]sendMultiTLV2MCUtest\n");
//        pTaskMngr->mTransportLayerProcessor.sendMultiTLV2MCUtest();//test sendMultiTLV2MCUtest



#ifdef NOTEST
        usleep(600*1000);//ms
#else
        sleep(2);
#endif

    }

}

void* taskManager::taskSendSerialMsg(void *arg){
    unsigned char rawData[FramLenMax];
    int dataLen;
    int msgQueueLength;
    int ret;
    int clientRWS;
    struct timespec ts;
    bool waitACK=false;
    unsigned sleepTime=100*1000;//ms
    UART_Dbg("[start] taskSendSerialMsg\n");
    UNUSED(arg);

    while(TRUE)
    {
        if(waitACK){//等待同步信号
            //设置等待时间为80ms
            if (clock_gettime( CLOCK_REALTIME,&ts ) < 0 ){
                usleep(sleepTime);
                UART_Err("clock_gettime failed\n");
                continue;
            }
            ts.tv_nsec += nano_sec;ts.tv_sec += ts.tv_nsec/NSECTOSEC;ts.tv_nsec = ts.tv_nsec%NSECTOSEC;
            ret = sem_timedwait(&g_semaphore,&ts);
            if (ret == -1) {//超时处理
                if (errno == ETIMEDOUT)
                    UART_Err("sem_timedwait() timed out\n");
                else
                    perror("sem_timedwait");
                usleep(sleepTime);//ms
                //[todo]重新请求数据
                continue;
            }
            //收到同步信号
            //判断对方接受区是否已经满了
            clientRWS=getClientRWS();
            if(clientRWS<1) continue;
        }else{
            usleep(sleepTime);
        }
        msgQueueLength = g_MsgQueueSend->Queuelength();
//        UART_Dbg("[--] taskSendSerialMsg queueLen=%d\n",msgQueueLength);
        if(msgQueueLength){
            g_MsgQueueSend->Dequeue(rawData,dataLen);
            waitACK=rawData[dataLen-1];
            UART_Dbg("RWS[%d] Queue[%d] SendData is:",clientRWS,msgQueueLength);for(int i =0;i<dataLen;i++){printf("0x%02x ",rawData[i]);}printf("\n");
            ret = g_serialCom->Write(rawData,dataLen-1);
            if(!ret) UART_Err("[taskSendSerialMsg] send data error!\n");
            //判断发送出去的消息是否需要等待ACK，需要的话则等待
        }

    }
}

void taskManager::initSystem(){
    int ret=-1;
    ret = sem_init(&g_semaphore,0,0);//If pshared has the value 0, then the semaphore is shared between the threads of a process
    if(ret) UART_Err("[Error] sem_init failed\n");
}

void taskManager::start(){
    int ret=-1;
    ret = serialComInit();
    while(!ret){
        UART_Err("[Error] serialComInit failed\n");
        sleep(1);
        ret = serialComInit();
    }
    ret = pthread_create(&serialRecv, NULL,taskRecvSerialMsg, this);
    if(ret) UART_Err("[Error] start taskRecvSerialMsg failed\n");
    ret = pthread_create(&serialProcessor, NULL,taskProcessSerialMsg, this);
    if(ret) UART_Err("[Error] start taskProcessSerialMsg failed\n");//若成功则返回0，否则返回出错编号
    ret = pthread_create(&serialSend, NULL,taskSendSerialMsg, this);
    if(ret) UART_Err("[Error] start taskSendSerialMsg failed\n");
    pthread_join(serialProcessor,NULL);


}

