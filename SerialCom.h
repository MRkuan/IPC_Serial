/***************************************************
* @filename  serialcom.h
* @brief 以面向对象的方式定义串口类
* @author fanjiafei
* @date 2017-11-1
* @version V0.1
****************************************************/

#ifndef SERIALCOM_H
#define SERIALCOM_H
#include <pthread.h>
#include <string>
#include <unistd.h>



class serialCom
{
public:
    serialCom();
    ~serialCom();
    int open_port(const char* device);
    int close_port(int fd);
    int setup_port(int fd, int speed, int data_bits, char parity, int stop_bits);
    int SerialInit(const char* device,int speed, int data_bits, char parity, int stop_bits);
    int RecvData(unsigned char *recvData);
    int SendData(unsigned char *SendData,unsigned int SendDataLen);
//    static serialCom* getInstance();

private:

    int gSerialFd;

//    static serialCom* instance_serialCom;

};

#endif // SERIALCOM_H
