
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
    unsigned char testRawData[20]={0xec,0xbc,0x40,0x40,0xff};
    unsigned char encodeData[20]={0};
    COBSTool cobs;
    cobs.StuffData(testRawData,6,encodeData);
    for(int i =0;i<6;i++){
        printf("0x%02x ",encodeData[i]);
    }

}


#endif


