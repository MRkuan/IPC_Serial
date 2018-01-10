#include "common.h"

void toHexChar(unsigned char c, char *h4, char *l4)
{
    int base0 = 48;
    int baseA = 97;
    int hc = c >> 4 & 0xF;
    int lc = c & 0xF;
    if (hc < 10)
    {
        *h4 = hc + base0;
    }
    else
    {
        *h4 = hc % 10 + baseA;
    }
    if (lc < 10)
    {
        *l4 = lc + base0;
    }
    else
    {
        *l4 = lc % 10 + baseA;
    }
}

void printHex(unsigned char *data_buf, ssize_t len, const char *prefixStr)
{
    int i;
    int size = len * 2 + 1;
    char printable[size];
    char h4;
    char l4;
    for (i = 0; i < len; ++i)
    {
        toHexChar(data_buf[i], &h4, &l4);
        printable[i * 2] = h4;
        printable[i * 2 + 1] = l4;
    }
    printable[size - 1] = 0;
    UART_Dbg("%s %d %s\n", prefixStr, len, printable);
}


/**
 * @brief GetFCS_8
 * @param pbuf
 * @param len
 * @return
 * 获取一个1B的FCS校验和
 */
unsigned char GetFCS_8(unsigned char *pbuf, unsigned int len)
{
    unsigned int i;
    unsigned char tmpFCS = 0;

    for (i = 0; i < len; i++)
    {
        tmpFCS ^= pbuf[i];
    }
    return tmpFCS;
}



serialCom* getSerialCom(){
    if(g_serialCom==NULL){
        g_serialCom = new serialCom();
    }
    return g_serialCom;
}

MsgQueue* getMsgQueue(){
    if(g_MsgQueue==NULL){
        g_MsgQueue = new MsgQueue();
    }
    return g_MsgQueue;
}