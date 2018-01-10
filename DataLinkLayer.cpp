#include "DataLinkLayer.h"
#include "common.h"
extern MsgQueue* m_MsgQueue;
DataLinkLayer::DataLinkLayer()
{
    if(g_MsgQueue==NULL){
        g_MsgQueue = getMsgQueue();
    }
}

DataLinkLayer::~DataLinkLayer()
{

}
/**
 * @brief DataLinkLayer::splitDLFrame
 * function：数据链路层解包Data Link layer(DL)
 * 解包，防止出现接收一次的数据buf里面包含多包数据的情况，以及上一包数据需和下一包数据拼包的情况，将解好的数据包放入msqQueue
 * 队列里DL层数据结构为：Frame_Header+Packet_Header+APP_Data+FCS+Frame_Tail
 * @param buf
 * @param datalength
 */
void DataLinkLayer::splitDLFrame(unsigned char* buf,int datalength){
    static bool needRecombine = 0;//是否需要与上一包组包
    static char unsigned mediaBuf[FramLenMax];//缓存媒介
    static char mediaBufIndex=0;//
    unsigned short headTailCout=0;//头尾帧计数
    unsigned int headIndex=0;//头位置
    unsigned int tailIndex = 0;//
    for(int i =0;i<datalength;i++){
        if(Frame_Head_Tail_Recv==buf[i]){
            headTailCout++;
            if(needRecombine&&(1==headTailCout)){//需与上一包组包
                needRecombine=0;
                tailIndex = i;
                memcpy(mediaBuf+mediaBufIndex,buf,tailIndex+1);
                mediaBufIndex+=(tailIndex+1);
                //将mediaBuf中数据enqueue到队列中,长度为mediaBufIndex
                g_MsgQueue->Enqueue(mediaBuf,mediaBufIndex);
                headTailCout=0;
                mediaBufIndex = 0;
            }else if(1==headTailCout%2){
                headIndex = i;
            }else if(0==headTailCout%2){
                if(1==i-headIndex){//若连续两个0x00，头指针后移
                    headIndex = i;
                    headTailCout--;
                }
                else{
                    tailIndex = i;
                    memcpy(mediaBuf,buf+headIndex,tailIndex-headIndex);//headIndex到i之间就是一组数据
                    //将mediaBuf中数据enqueue到队列中,长度为tailIndex-headIndex
                    g_MsgQueue->Enqueue(mediaBuf,tailIndex-headIndex);
                    mediaBufIndex = 0;
                }
            }
        }
    }
    if(Frame_Head_Tail_Recv!=buf[datalength-1]){
        if(1==headTailCout){//若只有一个0x00，取其后半截
            tailIndex = headIndex;
        }
        needRecombine = 1;
        mediaBufIndex = (datalength-1)-tailIndex;
        memcpy(mediaBuf,buf+headIndex,mediaBufIndex);
    }
}