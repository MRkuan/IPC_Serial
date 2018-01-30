
#include "taskManager.h"
#include <qfile.h>
#include <qdebug.h>

#if 1
int main(){
    taskManager m_taskManager;
    m_taskManager.start();
    return 0;
}

#else
#include "COBSTool.h"
#include "stdio.h"
int main(){
    unsigned char testRawData[12]={0x45,0x00,0x00,0x2c,0x4c,0x79,0x00,0x00,0x40,0x06,0x4f,0x37};
    unsigned char testRawData2[13]={0x45,0x00,0x00,0x2c,0x4c,0x79,0x00,0x00,0x40,0x06,0x4f,0x37,0x00};
    unsigned char encodeData[20]={9};
    int len;
    COBSTool cobs;

    len =13;//转译后长度为转译前长度+1
    cobs.StuffData(testRawData,len,encodeData);
    printf("\nstuff\n");
    for(int i =0;i<len;i++){
        printf("0x%02x ",encodeData[i]);
    }
    cobs.UnStuffData(encodeData,len-1,testRawData);//转译前的长度为转译后的长度-1
    printf("\nunstuff\n");
    for(int i =0;i<len-1;i++){
        printf("0x%02x ",testRawData[i]);
    }
    len =14;
    printf("\nstuff\n");
    cobs.StuffData(testRawData2,len,encodeData);
    for(int i =0;i<len;i++){
        printf("0x%02x ",encodeData[i]);
    }
    cobs.UnStuffData(encodeData,len-1,testRawData2);
    printf("\nunstuff\n");
    for(int i =0;i<len-1;i++){
        printf("0x%02x ",testRawData2[i]);
    }
}


#endif


