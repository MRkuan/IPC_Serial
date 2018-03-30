#include "TransportLayer.h"
extern serial* g_serialCom;
TransportLayer::TransportLayer()
{
    if(g_serialCom==NULL){
        g_serialCom = getSerialCom();
    }
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

TransportLayer::~TransportLayer()
{

}
/**
 * @brief TransportLayer::sendMultiTLV2MCUtest
 * @details 发送多TLV消息测试
 */
void TransportLayer::sendMultiTLV2MCUtest(){
    U_PacketHeader ph;
    S_TLV tlv[2];
    unsigned char value[3]={0x11,0x22,0x33};
    unsigned char value2[5]={0x44,0x55,0x66,0x77,0x88};
    ph.Packet_Header.PT=0;
    ph.Packet_Header.CID=1;
    ph.Packet_Header.SN=getSN();
    ph.Packet_Header.Reserved=0;
    ph.Packet_Header.LENapp=(sizeof(tlv[0].tag)+1)*2+3+5;

    tlv[0].tag.Tag.BusinessType=4;
    tlv[0].tag.Tag.BusinessSub1Type=5;
    tlv[0].tag.Tag.BusinessSub2Type=6;
    tlv[0].len=3;
    tlv[0].value=value;

    tlv[1].tag.Tag.BusinessType=7;
    tlv[1].tag.Tag.BusinessSub1Type=8;
    tlv[1].tag.Tag.BusinessSub2Type=9;
    tlv[1].len=5;
    tlv[1].value=value2;
    sendMultiTLV2MCU(ph,tlv,2);
}

/**
 * @brief TransportLayer::sendSingleTLV2MCUtest
 * @details 发送单TLV消息测试
 */
void TransportLayer::sendSingleTLV2MCUtest(){
    U_PacketHeader ph;
    S_TLV tlv;
    unsigned char value[3]={0x11,0x22,0x33};
    ph.Packet_Header.PT=0;
    ph.Packet_Header.CID=1;
    ph.Packet_Header.SN=getSN();
    ph.Packet_Header.Reserved=0;
    ph.Packet_Header.LENapp=sizeof(tlv.tag)+1+3;
    tlv.tag.Tag.BusinessType=4;
    tlv.tag.Tag.BusinessSub1Type=5;
    tlv.tag.Tag.BusinessSub2Type=6;
    tlv.len=3;
    tlv.value=value;
    sendSingleTLV2MCU(ph,tlv);
}

/**
 * @brief TransportLayer::sendMultiTLV2MCU
 * @details 发送多TLV消息
 * @param m_ph 包头
 * @param m_tlv tlv数组首地址
 * @param TLVCount tlv个数
 * 发送多TLV消息接口
 */
void TransportLayer::sendMultiTLV2MCU(U_PacketHeader& m_ph,S_TLV m_tlv[],int TLVCount){
    unsigned char fcs;
    unsigned char rawbuf[FramLenMax];
    unsigned char encodedBuf[FramLenMax];
    unsigned int totalLen=0;
    unsigned int index=0;

    for(int i =0;i<TLVCount;i++){
        totalLen=totalLen+sizeof(m_tlv[i].tag)+sizeof(m_tlv[i].len)+m_tlv[i].len;
    }
    totalLen=totalLen+sizeof(U_PacketHeader)+4;//4:head+tail+fcs+COBS转译差位
    if(totalLen>FramLenMax)
    {
        UART_Err("[sendMultiTLV2MCU] length error len:%d\n",totalLen);
        return ;
    }

    memcpy(rawbuf,(void* )&m_ph,sizeof(m_ph));
    index+=sizeof(m_ph);
    for(int i=0;i<TLVCount;i++){
        memcpy(rawbuf+index,(void *)&(m_tlv[i].tag),sizeof(m_tlv[i].tag));
        index+=sizeof(m_tlv[i].tag);
        rawbuf[index]=m_tlv[i].len;
        index+=sizeof(m_tlv[i].len);
        memcpy(rawbuf+index,m_tlv[i].value,m_tlv[i].len);
        index+=m_tlv[i].len;
    }

    fcs = GetFCS_8(rawbuf,index);
    rawbuf[index++]=fcs;
    mCOBStool.StuffData(rawbuf,index,encodedBuf+1);
    index+=3;//add head tail COBS转译后长度+1
    encodedBuf[0]=Frame_Head_Tail_Send;
    encodedBuf[index-1]=Frame_Head_Tail_Send;
    if(index!=totalLen)
        UART_Err("[sendMultiTLV2MCU] length error totalLen:%d,index:%d\n",totalLen,index);
    //最后附加一位表示是否需要等待ACK，供发送进程taskSendSerialMsg作是否等待ACK的判断依据
    switch(m_ph.Packet_Header.PT){
    case MNA_Single_TLV:
    case MNA_Multi_TLV:
        encodedBuf[index]=1;
        break;
    default:
        encodedBuf[index]=0;
    }
//    if(index>0){
//        UART_Dbg("sendMultiTLV2MCU is :");
//        for(unsigned int i =0;i<index;i++){
//            printf("0x%02x ",encodedBuf[i]);
//        }
//        printf("\n");
//    }
    g_MsgQueueSend->Enqueue(encodedBuf,index+1);
    UART_Dbg("[end]sendMultiTLV2MCU \n");
    return;

}

/**
 * @brief TransportLayer::sendSingleTLV2MCU
 * @details 发送单TLV消息
 * @param m_ph 包头
 * @param m_tlv tlv数据首地址
 */
void TransportLayer::sendSingleTLV2MCU(U_PacketHeader& m_ph,S_TLV& m_tlv){
    unsigned char fcs;
    unsigned char rawbuf[FramLenMax];
    unsigned char encodedBuf[FramLenMax];
    unsigned int totalLen;
    unsigned int index=0;

    totalLen=sizeof(U_PacketHeader)+sizeof(m_tlv.tag)+sizeof(m_tlv.len)+m_tlv.len+4;//4:head+tail+fcs+COBS转译差位
    if(totalLen>FramLenMax)
    {
        UART_Err("[sendSingleTLV2MCU] length error len=%d\n",totalLen);
        return ;
    }

    memcpy(rawbuf,(void* )&m_ph,sizeof(m_ph));
    index+=sizeof(m_ph);
    memcpy(rawbuf+index,(void *)&(m_tlv.tag),sizeof(m_tlv.tag));
    index+=sizeof(m_tlv.tag);
    rawbuf[index]=m_tlv.len;
    index+=sizeof(m_tlv.len);
    memcpy(rawbuf+index,m_tlv.value,m_tlv.len);
    index+=m_tlv.len;
    fcs = GetFCS_8(rawbuf,index);
    rawbuf[index++]=fcs;
    mCOBStool.StuffData(rawbuf,index,encodedBuf+1);
    index+=3;//add head tail COBS转译后长度+1
    encodedBuf[0]=Frame_Head_Tail_Send;
    encodedBuf[index-1]=Frame_Head_Tail_Send;
    if(index!=totalLen)
        UART_Err("[sendSingleTLV2MCU] length error totalLen:%d,index:%d\n",totalLen,index);
    //最后附加一位表示是否需要等待ACK，供发送进程taskSendSerialMsg作是否等待ACK的判断依据
    switch(m_ph.Packet_Header.PT){
    case MNA_Single_TLV:
    case MNA_Multi_TLV:
        encodedBuf[index]=1;
        break;
    default:
        encodedBuf[index]=0;
    }
//    if(index>0){
//        UART_Dbg("sendSingleTLV2MCU is :\n");
//        for(int i =0;i<index;i++){
//            printf("0x%02x ",encodedBuf[i]);
//        }
//        printf("\n");
//    }
    g_MsgQueueSend->Enqueue(encodedBuf,index+1);
//    UART_Dbg("[end]sendSingleTLV2MCU \n");
    return;

}
/**
 * @brief TransportLayer::replyACK
 * @details 回复ACK
 * @param m_Ack
 */
void TransportLayer::replyACK(U_ACKpacket m_Ack){
    unsigned char fcs;
    unsigned char rawbuf[FramLenMax];
    unsigned char encodedBuf[FramLenMax];
    unsigned int len;

    fcs = GetFCS_8((unsigned char *)&m_Ack,sizeof(m_Ack));
    len=sizeof(m_Ack)+sizeof(fcs);
    memcpy(rawbuf,(void*)&m_Ack,sizeof(m_Ack));
    rawbuf[len-1]=fcs;

    mCOBStool.StuffData(rawbuf,++len,encodedBuf+1);//cobs转译后长度+1
    len+=2;
    encodedBuf[0]=Frame_Head_Tail_Send;
    encodedBuf[len-1]=Frame_Head_Tail_Send;
    encodedBuf[len]=0;  //最后附加一位表示是否需要等待ACK，供发送进程taskSendSerialMsg作是否等待ACK的判断依据
    if(len>0){
        UART_Dbg("replyACK is :");
        for(unsigned int i =0;i<len;i++){
            printf("0x%02x ",encodedBuf[i]);
        }
        printf("\n");
    }
    g_MsgQueueSend->Enqueue(encodedBuf,len+1);
    UART_Dbg("[end]replyACK \n");
}


/**
 * @brief TransportLayer::splitTPData
 * @details 传输层对数据进行解包 TP层数据结构为：Packet_Header+APP_Data
 * @param buf  数据首地址
 * @param datalen 数据长度
 */
void TransportLayer::splitTPData(unsigned char* buf,unsigned int datalen){
    U_PacketHeader uph;
    U_ACKpacket * pUAckPh;
    static unsigned char appDataBuff[FramLenMax];
    int ret;
    UART_Dbg("AnalyseData is :");
    for(unsigned int i =0;i<datalen;i++){
        printf("0x%02x ",buf[i]);
    }
    printf("\n");

    //analyse Packet_Header
    memcpy((void*)&uph,buf,sizeof(U_PacketHeader));
    UART_Dbg("PT:%d,CID:%d SN:%d \n",uph.Packet_Header.PT,uph.Packet_Header.CID,uph.Packet_Header.SN);

    //reply ACK
    U_ACKpacket m_Ack;
    m_Ack.ACK_Packet.PT=ACK;
    m_Ack.ACK_Packet.CID = uph.Packet_Header.CID;
    m_Ack.ACK_Packet.ACK_SN = uph.Packet_Header.SN;
    m_Ack.ACK_Packet.RWS=g_MsgQueueRecv->getQueueRWS()-1;

    switch(uph.Packet_Header.PT){
    case MNA_Single_TLV://仅包含单个业务类(表现为只有1个TLV字段)的重要数据；需接收方回复ACK或流控消息; LENapp>0;
        if(datalen!=(uph.Packet_Header.LENapp+sizeof(U_PacketHeader)))
        {
            UART_Err("[MNA] length error,recv datalen:%d,theoretical len:%d\n",datalen,uph.Packet_Header.LENapp+sizeof(U_PacketHeader));
            break;
        }
        memcpy(appDataBuff,buf+sizeof(U_PacketHeader),uph.Packet_Header.LENapp);
        mTLVtools.singleTLVRevProcessor(appDataBuff,uph.Packet_Header.LENapp);
        m_Ack.ACK_Packet.ET=0;

        UART_Dbg("ACKinfo PT:%d CID:%d SN:%d myRWS:%d\n",m_Ack.ACK_Packet.PT,m_Ack.ACK_Packet.CID,m_Ack.ACK_Packet.ACK_SN,m_Ack.ACK_Packet.RWS);
        replyACK(m_Ack);
        break;

    case MNDA_Single_TLV://仅包含单个业务类(表现为只有1个TLV字段)非重要数据；无需接收方回复ACK或流控消息; LENapp>0;
        if(datalen!=(uph.Packet_Header.LENapp+sizeof(U_PacketHeader)))
        {
            UART_Err("[MNDA] length error,recv datalen:%d,theoretical len:%d\n",datalen,uph.Packet_Header.LENapp+sizeof(U_PacketHeader));
            break;
        }
        //[todo]
        memcpy(appDataBuff,buf+sizeof(U_PacketHeader),uph.Packet_Header.LENapp);
        mTLVtools.singleTLVRevProcessor(appDataBuff,uph.Packet_Header.LENapp);
        break;

    case ACK://应答；Packet_Header后没有内容，即整个TP_PACKET就只包含Packet_Header，没有APP_DATA;此时的Packet Header组成会有一定改变
        UART_Dbg("recv ack\n");
        pUAckPh = (U_ACKpacket *)&uph;
        UART_Dbg("[ACK] ClientRWS:%d ACK_SN:%d ET:%d \n",pUAckPh->ACK_Packet.RWS,pUAckPh->ACK_Packet.ACK_SN,pUAckPh->ACK_Packet.ET);
        setClientRWS(pUAckPh->ACK_Packet.RWS);
        if (sem_post(g_semaphore) == -1) UART_Err("sem_post() failed\n");
        break;
    case Reserved1://预留
        break;
    case MNA_Multi_TLV://包含多个业务类(表现为多个TLV字段)的重要数据，需发送方回复ACK; LENapp>0;
        memcpy(appDataBuff,buf+sizeof(U_PacketHeader),uph.Packet_Header.LENapp);
        ret = mTLVtools.multiTLVRecvProcessor(appDataBuff,uph.Packet_Header.LENapp);
        if(ret){
            //[todo] add error reply
        }else{
            m_Ack.ACK_Packet.ET=0;
            UART_Dbg("ACKinfo PT:%d CID:%d SN:%d\n myRWS:%d\n",m_Ack.ACK_Packet.PT,m_Ack.ACK_Packet.CID,m_Ack.ACK_Packet.ACK_SN,m_Ack.ACK_Packet.RWS);
            replyACK(m_Ack);
        }
        break;
    case MNDA_Multi_TLV://包含多个业务类(表现为多个TLV字段)的不重要数据; LENapp>0;
        memcpy(appDataBuff,buf+sizeof(U_PacketHeader),uph.Packet_Header.LENapp);
        ret = mTLVtools.multiTLVRecvProcessor(appDataBuff,uph.Packet_Header.LENapp);
        if(ret){
            //[todo] add error reply
        }
        break;
    case Reserved2://预留
        break;
    default:
        UART_Dbg("msg packheader:%d\n",uph.Packet_Header.PT);
        break;
    }
}
