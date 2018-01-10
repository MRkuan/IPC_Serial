#include "TransportLayer.h"
extern serialCom* g_serialCom;
TransportLayer::TransportLayer()
{
    if(g_serialCom==NULL){
        g_serialCom = getSerialCom();
    }
}

TransportLayer::~TransportLayer()
{

}


void TransportLayer::replyACK(U_ACKpacket m_Ack){
    unsigned char fcs;
    unsigned char rawbuf[FramLenMax];
    unsigned char encodedBuf[FramLenMax];
    unsigned int len;

    fcs = GetFCS_8((unsigned char *)&m_Ack,sizeof(m_Ack));
    len=sizeof(m_Ack)+sizeof(fcs);
    memcpy(rawbuf,(void*)&m_Ack,sizeof(m_Ack));
    rawbuf[sizeof(m_Ack)]=fcs;

    mCOBStool.StuffData(rawbuf,len,encodedBuf+1);
    encodedBuf[0]=Frame_Head_Tail_Send;
    len+=2;
    encodedBuf[len-1]=Frame_Head_Tail_Send;
    if(len>0){
        UART_Dbg("\nreplyACK is :");
        for(int i =0;i<len;i++){
            printf("0x%02x ",encodedBuf[i]);
        }
        printf("\n");
    }
    g_serialCom->SendData(encodedBuf,len);
    UART_Dbg("[end]replyACK \n");
}


/**
 * @brief TransportLayer::splitTPData
 * TP层数据结构为：Packet_Header+APP_Data
 * @param buf
 * @param datalen
 */
void TransportLayer::splitTPData(unsigned char* buf,unsigned int datalen){
    U_PacketHeader uph;
    static unsigned char appDataBuff[FramLenMax];
    int ret;
    UART_Dbg("\nAnalyseData is :");
    for(int i =0;i<datalen;i++){
        printf("0x%02x ",buf[i]);
    }
    printf("\n");

    //analyse Packet_Header
    memcpy((void*)&uph,buf,sizeof(U_PacketHeader));
    UART_Dbg("PT:%d\n",uph.Packet_Header.PT);
    switch(uph.Packet_Header.PT){
    case MNA_Single_TLV://仅包含单个业务类(表现为只有1个TLV字段)的重要数据；需接收方回复ACK或流控消息; LENapp>0;
        if(datalen!=(uph.Packet_Header.LENapp+sizeof(U_PacketHeader)))
        {
            UART_Err("[MNA] length error,recv datalen:%d,theoretical len:%d\n",datalen,uph.Packet_Header.LENapp+sizeof(U_PacketHeader));
            break;
        }
        memcpy(appDataBuff,buf+sizeof(U_PacketHeader),uph.Packet_Header.LENapp);
        mTLVtools.singleTLVRevProcessor(appDataBuff,uph.Packet_Header.LENapp);
        //reply ACK
        U_ACKpacket m_Ack;
        m_Ack.ACK_Packet.PT=ACK;
        m_Ack.ACK_Packet.CID = uph.Packet_Header.CID;
        m_Ack.ACK_Packet.ACK_SN = uph.Packet_Header.SN;
        UART_Dbg("ACKinfo PT:%d CID:%d SN:%d\n",m_Ack.ACK_Packet.PT,m_Ack.ACK_Packet.CID,m_Ack.ACK_Packet.ACK_SN);
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
        UART_Dbg("Recv ACK\n");
        break;
    case Reserved1://预留
        break;
    case MNA_Multi_TLV://包含多个业务类(表现为多个TLV字段)的重要数据，需发送方回复ACK; LENapp>0;
        memcpy(appDataBuff,buf+sizeof(U_PacketHeader),uph.Packet_Header.LENapp);
        ret = mTLVtools.multiTLVRecvProcessor(appDataBuff,uph.Packet_Header.LENapp);
        if(!ret){
            //[todo] add error reply
        }else{
            U_ACKpacket m_Ack;
            m_Ack.ACK_Packet.PT=ACK;
            m_Ack.ACK_Packet.CID = uph.Packet_Header.CID;
            m_Ack.ACK_Packet.ACK_SN = uph.Packet_Header.SN;
            UART_Dbg("ACKinfo PT:%d CID:%d SN:%d\n",m_Ack.ACK_Packet.PT,m_Ack.ACK_Packet.CID,m_Ack.ACK_Packet.ACK_SN);
            replyACK(m_Ack);
        }
        break;
    case MNDA_Multi_TLV://包含多个业务类(表现为多个TLV字段)的不重要数据; LENapp>0;
        memcpy(appDataBuff,buf+sizeof(U_PacketHeader),uph.Packet_Header.LENapp);
        ret = mTLVtools.multiTLVRecvProcessor(appDataBuff,uph.Packet_Header.LENapp);
        if(!ret){
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
