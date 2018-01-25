#include "TLVtools.h"

TLVtools::TLVtools()
{

}

TLVtools::~TLVtools()
{

}

int TLVtools::singleTLVRevProcessor(unsigned char* appDataBuff,unsigned short totallen){
    S_TLV mTLV;
    memcpy((char*)&(mTLV.tag),appDataBuff,sizeof(mTLV.tag));
    memcpy((char*)&(mTLV.len),appDataBuff+sizeof(mTLV.tag),sizeof(mTLV.len));
    mTLV.value = appDataBuff+sizeof(mTLV.tag)+sizeof(mTLV.len);
    UART_Dbg("\nTLVinfo tag:%d tag1:%d tag2:%d len:%d \n",\
           mTLV.tag.Tag.BusinessType,mTLV.tag.Tag.BusinessSub1Type,mTLV.tag.Tag.BusinessSub2Type,mTLV.len);
    if(mTLV.len!=totallen-(sizeof(mTLV.tag)+sizeof(mTLV.len))){
        UART_Dbg("[error]single TLV with error totallen%d TLVlen:%d\n",totallen,mTLV.len);
        return -1;
    }
    UART_Dbg("\nAPP DataContent is :");
    for(int i =0;i<mTLV.len;i++){
        printf("0x%02x ",mTLV.value[i]);
    }
    printf("\n");
    return 0;
}

int TLVtools::multiTLVRecvProcessor(unsigned char* appDataBuff,unsigned short totallen){
    S_TLV mTLV;
    unsigned short processedLen;

    memcpy((char*)&(mTLV.tag),appDataBuff,sizeof(mTLV.tag));
    memcpy((char*)&(mTLV.len),appDataBuff+sizeof(mTLV.tag),sizeof(mTLV.len));
    mTLV.value = appDataBuff+sizeof(mTLV.tag)+sizeof(mTLV.len);
    UART_Dbg("\nTLVinfo tag:%d tag1:%d tag2:%d len:%d \n",\
           mTLV.tag.Tag.BusinessType,mTLV.tag.Tag.BusinessSub1Type,mTLV.tag.Tag.BusinessSub2Type,mTLV.len);
    if(totallen>FramLenMax||mTLV.len>totallen){
        UART_Dbg("[error]multiTLVs with error len totallen%d mTLVlen:%d\n",totallen,mTLV.len);
        return -1;
    }
    UART_Dbg("\nAPP DataContent is :");
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
}

int TLVtools::singleTLVSendProcessor(U_Tag appDataTag,unsigned short RawAppDatalen,unsigned char* RawAppDataBuf,unsigned char* TLVappDataBuf,unsigned short &totallen){

    memcpy((void*)TLVappDataBuf,(void*)&appDataTag,sizeof(appDataTag));
    memcpy(TLVappDataBuf+sizeof(appDataTag),(void*)&RawAppDatalen,sizeof(RawAppDatalen));
    memcpy(TLVappDataBuf+sizeof(appDataTag)+sizeof(RawAppDatalen),(void *)RawAppDataBuf,RawAppDatalen);
    totallen = sizeof(appDataTag)+sizeof(RawAppDatalen)+RawAppDatalen;
    return 0;
}


