/****************************************************************
****************************************************************/
#ifndef __COMMON_H
#define __COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <linux/input.h>
#include "MsgQueue.h"
#include "serial.h"



/*-------------------color-------------------------------------*/
#define NONE         "\033[m"
#define RED          "\033[0;32;31m"
#define LIGHT_RED    "\033[1;31m"
#define GREEN        "\033[0;32;32m"
#define LIGHT_GREEN  "\033[1;32m"
#define BLUE         "\033[0;32;34m"
#define LIGHT_BLUE   "\033[1;34m"
#define DARY_GRAY    "\033[1;30m"
#define CYAN         "\033[0;36m"
#define LIGHT_CYAN   "\033[1;36m"
#define PURPLE       "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN        "\033[0;33m"
#define YELLOW       "\033[1;33m"
#define LIGHT_GRAY   "\033[0;37m"
#define WHITE        "\033[1;37m"
/*--------------------color------------------------------------*/


/**
 * print
 */
#define DEBUG_SWITCH        1
#ifdef  DEBUG_SWITCH
#define UART_Dbg(fmt,args...) printf(LIGHT_GREEN "[UART] " NONE fmt , ##args)
#else
#define UART_Dbg(fmt,args...) /*do nothing */
#endif


#define   ERR_DEBUG_SWITCH        1
#ifdef    ERR_DEBUG_SWITCH
#define UART_Err(fmt,args...) printf(RED "[UART][ERROR] " NONE "\nFile:<%s> Fun:[%s] Line:%d\n "fmt, __FILE__, __FUNCTION__, __LINE__, ##args)
#else
#define UART_Err(fmt,args...) /*do nothing */
#endif


#define Frame_Head_Tail_Send 0xff
#define Frame_Head_Tail_Recv 0x00
#define TRUE 1
#define FALSE 0
const int FramLenMax=75;
static serial* g_serialCom;
static MsgQueue* g_MsgQueue;

union U_PacketHeader{
    struct{
        unsigned int LENapp:12;  //20~31
        unsigned int Reserved:4; //16~19
        unsigned int SN:6;       //10~15
        unsigned int CID:7;      //3~9
        unsigned int PT:3;       //0~2//上左下右
    }Packet_Header;
    unsigned int value;
};


union U_ACKpacket{
    struct{
        unsigned int RWS:12;  //20~31
        unsigned int ET:4;      //16~19
        unsigned int ACK_SN:6;   //10~15
        unsigned int CID:7;      //3~9
        unsigned int PT:3;       //0~2//上左下右
    }ACK_Packet;
        unsigned int value;
};

union U_Tag{
    struct{
        unsigned short BusinessSub2Type:5;
        unsigned short BusinessSub1Type:6;
        unsigned short BusinessType:5;
    }Tag;
    unsigned short value;
};


struct S_TLV{
    U_Tag tag;
    unsigned short len;
    unsigned char* value;
};


enum Packet_Type{
    MNA_Single_TLV=0,
    MNDA_Single_TLV,
    ACK,
    Reserved1,
    MNA_Multi_TLV,
    MNDA_Multi_TLV,
    Reserved2
};

enum Error_Type{
    OK=0,
    DataLengthError,
    FCSError,
    BufferInsufficient,
    Reserved
};




void toHexChar(unsigned char c, char *h4, char *l4);

void printHex(unsigned char *data_buf, ssize_t len, const char *prefixStr);

unsigned char GetFCS_8(unsigned char *pbuf, unsigned int len);

serial* getSerialCom();

MsgQueue* getMsgQueue();
#endif	//__COMMON_H
