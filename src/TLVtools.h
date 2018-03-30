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
    void TLVTable(unsigned short tag1,unsigned short tag2,unsigned short tag3,unsigned char len,unsigned char* dataValue);


};

#endif // TLVTOOLS_H
