#ifndef TLVTOOLS_H
#define TLVTOOLS_H
#include "common.h"

class TLVtools
{
public:
    TLVtools();
    ~TLVtools();
    int singleTLVRevProcessor(unsigned char* sTLVs,unsigned short totallen);
    int multiTLVRecvProcessor(unsigned char* sTLVs,unsigned short totallen);
    int singleTLVSendProcessor(U_Tag appDataTag,unsigned short RawAppDatalen,unsigned char* RawAppDataBuf,unsigned char* TLVappDataBuf,unsigned short &totallen);
};

#endif // TLVTOOLS_H
