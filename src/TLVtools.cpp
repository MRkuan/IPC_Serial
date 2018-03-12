#include "TLVtools.h"
#include "SerialServer.h"
TLVtools::TLVtools()
{

}

TLVtools::~TLVtools()
{

}
/**
 * @brief TLVtools::singleTLVRevProcessor
 * @param appDataBuff 数据地址
 * @param totallen 数据长度
 * 解析接收到的单包TLV
 * @return
 */
int TLVtools::singleTLVRevProcessor(unsigned char* appDataBuff,unsigned short totallen){
    S_TLV mTLV;
    memcpy((char*)&(mTLV.tag),appDataBuff,sizeof(mTLV.tag));
    memcpy((char*)&(mTLV.len),appDataBuff+sizeof(mTLV.tag),sizeof(mTLV.len));
    mTLV.value = appDataBuff+sizeof(mTLV.tag)+sizeof(mTLV.len);
    UART_Dbg("TLVinfo tag:%d tag1:%d tag2:%d len:%d \n",\
           mTLV.tag.Tag.BusinessType,mTLV.tag.Tag.BusinessSub1Type,mTLV.tag.Tag.BusinessSub2Type,mTLV.len);
    if(mTLV.len!=totallen-(sizeof(mTLV.tag)+sizeof(mTLV.len))){
        UART_Dbg("[error]single TLV with error totallen%d TLVlen:%d\n",totallen,mTLV.len);
        return -1;
    }
    UART_Dbg("APP DataContent is :");for(int i =0;i<mTLV.len;i++){printf("0x%02x ",mTLV.value[i]);}printf("\n");
    TLVTable((unsigned short )mTLV.tag.Tag.BusinessType,(unsigned short )mTLV.tag.Tag.BusinessSub1Type,(unsigned short )mTLV.tag.Tag.BusinessSub2Type,(unsigned short )mTLV.len,mTLV.value);
    return 0;
}

/**
 * @brief TLVtools::multiTLVRecvProcessor
 * @param appDataBuff 数据地址
 * @param totallen 数据长度
 * 解析接收到的多包TLV
 * @return
 */
int TLVtools::multiTLVRecvProcessor(unsigned char* appDataBuff,unsigned short totallen){
    S_TLV mTLV;
    unsigned short processedLen;

    memcpy((char*)&(mTLV.tag),appDataBuff,sizeof(mTLV.tag));
    memcpy((char*)&(mTLV.len),appDataBuff+sizeof(mTLV.tag),sizeof(mTLV.len));
    mTLV.value = appDataBuff+sizeof(mTLV.tag)+sizeof(mTLV.len);
    UART_Dbg("TLVinfo tag:%d tag1:%d tag2:%d len:%d \n",\
           mTLV.tag.Tag.BusinessType,mTLV.tag.Tag.BusinessSub1Type,mTLV.tag.Tag.BusinessSub2Type,mTLV.len);
    if(totallen>FramLenMax||mTLV.len>totallen){
        UART_Dbg("[error]multiTLVs with error len totallen%d mTLVlen:%d\n",totallen,mTLV.len);
        return -1;
    }
    UART_Dbg("APP DataContent is :");
    for(int i =0;i<mTLV.len;i++){
        printf("0x%02x ",mTLV.value[i]);
    }
    printf("\n");
    processedLen=sizeof(mTLV.tag)+sizeof(mTLV.len)+mTLV.len;

    if(processedLen<totallen){
        multiTLVRecvProcessor(appDataBuff+processedLen,totallen-processedLen);
    }else if(processedLen==totallen){
        UART_Dbg("multiTLVs with correct length\n");
        return 0;
    }else{
        UART_Dbg("[error]multiTLVs with error length\n");
        return -1;
    }
    return 0;
}
/**
 * @brief TLVtools::TLVTable
 * @param tag1
 * @param tag2
 * @param tag3
 * @param len
 * @param dataValue
 * TLV消息详细定义表
 */
void TLVtools::TLVTable(unsigned short tag1,unsigned short tag2,unsigned short tag3,unsigned char len,unsigned char* dataValue){
    SerialServer mSerialServer;
    QByteArray RawData(QByteArray::fromRawData((char *)dataValue, len));
    UART_Dbg("[TLVTable] TLVinfo tag:%d tag1:%d tag2:%d len:%d \n",tag1,tag2,tag3,len);
    for(int i =0;i<len;i++) printf("0x%x ",dataValue[i]);printf("\n");
    switch(tag1){
    case 0x0:
        switch (tag2) {
        case 0:
            switch (tag3) {
            case 0:
            case 1:
                break;
            default:
                break;
            }
        case 1:
        case 2:
        case 3:
            break;
        default:
            break;
        }
    case 0x1:
    case 0x2:
        switch (tag2) {
        case 0:
            switch (tag3) {
            case 0:
            case 1:
                break;
            default:
                break;
            }
        case 1:
        case 2:
        case 3:
        case 0x10:
            switch (tag3) {
            case 0:
                UART_Dbg("[TLVTable2]TLVinfo tag:0x%x tag1:0x%x tag2:0x%x len:%d \n",tag1,tag2,tag3,len);
                mSerialServer.broadcastCANInfo(RawData,len);
                break;
            case 1:
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    case 0x3:
        break;
    default:
        break;
    }
}

