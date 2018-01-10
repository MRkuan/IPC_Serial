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
    TLVtools mTLVtools;
    COBSTool mCOBStool;
};

#endif // TRANSPORTLAYER_H
