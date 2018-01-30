#ifndef TRANSPORTLAYER_H
#define TRANSPORTLAYER_H
#include "TLVtools.h"
#include "COBSTool.h"
class TransportLayer
{
public:
    TransportLayer();
    ~TransportLayer();
    void replyACK(U_ACKpacket m_Ack);
    void splitTPData(unsigned char* buf,unsigned int datalen);
    void sendSingleTLV2MCU(U_PacketHeader& m_ph,S_TLV& m_tlv);
    void sendMultiTLV2MCU(U_PacketHeader& m_ph,S_TLV m_tlv[],int TLVCount);
    void sendSingleTLV2MCUtest();
    void sendMultiTLV2MCUtest();
    TLVtools mTLVtools;
    COBSTool mCOBStool;
};

#endif // TRANSPORTLAYER_H
