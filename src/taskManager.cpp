/***************************************************
* @filename  taskManager.cpp
* @brief
* @author fanjiafei
* @date 2017-11-1
* @version V0.1
****************************************************/

#include "taskManager.h"
#include "unistd.h"
extern serial* g_serialCom;
extern MsgQueue* g_MsgQueue;
taskManager::taskManager()
{

    if(g_MsgQueue==NULL){
        g_MsgQueue = getMsgQueue();
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
    if(g_serialCom==NULL){
        g_serialCom = getSerialCom();
    }
    ret = g_serialCom->Open("/dev/tty.usbserial",115200,8,NO,1);
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
        msgQueueLength = g_MsgQueue->Queuelength();
//        UART_Dbg("[--] taskProcessSerialMsg queueLen=%d\n",msgQueueLength);
        if(msgQueueLength){
            g_MsgQueue->Dequeue(rawData,dataLen);
//                        UART_Dbg("RawData is:");
//                        for(int i =0;i<dataLen;i++){
//                            printf("0x%02x ",rawData[i]);
//                        }
            //去除数据首尾的Frame_Header和Frame_Tail，数据形式是:Packet_Header+APP_Data+FCS
            dataLen-=2;//（head tail） 剩余为有效长度
            pTaskMngr->mCOBStool.UnStuffData(rawData+1,dataLen,DecodedData);
            dataLen--;//COBS源码比转译后编码短1
                        UART_Dbg("DecodedData is:");
                        for(int i =0;i<dataLen;i++){
                            printf("0x%02x ",DecodedData[i]);
                        }
                        printf("\n");

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
#ifdef NOTEST
        usleep(10*1000);//10ms
#else
        sleep(2);
#endif

    }

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
    pthread_join(serialProcessor,NULL);


}

