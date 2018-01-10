/***************************************************
* @filename  serialcom.cpp
* @brief
* @author fanjiafei
* @date 2017-11-1
* @version V0.1
****************************************************/

#include "SerialCom.h"
#include <termios.h>
#include "common.h"
serialCom::serialCom()
{
    gSerialFd=-1;
}

serialCom::~serialCom()
{

}


/**
 * @brief serialCom::open_port
 * @param device
 * @return 返回-1为打开失败
 */
int serialCom::open_port(const char* device)
{
    UART_Dbg("device :%s\n",device);
    gSerialFd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (-1 == gSerialFd)
         UART_Err("[ERROR] Unable to open the port\n");
    return gSerialFd;
}


/**
 * @brief serialCom::close_port
 * @param fd
 * @return 0:success
 */
int serialCom::close_port(int fd)
{
    int ret = -1;
    ret = close(fd);
    if(ret< 0)
    {
        UART_Err("[ERROR] Unable to close the port.");
        return ret;
    }
    UART_Dbg("Close the port success!\n");
    return 0;
}

/**
 * @brief serialCom::setup_port
 * @param fd
 * @param speed
 * @param data_bits
 * @param parity
 * @param stop_bits
 * @return 0:success
 */
int serialCom::setup_port(int fd, int speed, int data_bits, char parity, int stop_bits)
{
    int speed_arr[] = {B115200,B57600, B9600, B38400, B19200, B4800};
    int name_arr[] = {115200, 57600,9600, 38400, 19200, 4800};
    struct termios opt;
    int ret=-1;
    int i=0;
    int len=0;

    ret = tcgetattr(fd, &opt);		        /* get the port attr */
    if (ret < 0)
        UART_Err("Unable to get the attribute");

    opt.c_cflag |= (CLOCAL | CREAD);            /* enable the receiver, set local mode */
    opt.c_cflag &= ~CSIZE;			/* mask the character size bits*/

    /* baud rate */
    len = sizeof(speed_arr) / sizeof(int);
    for (i = 0; i < len; i++)
    {
        if (speed == name_arr[i])
        {
            cfsetispeed(&opt, speed_arr[i]);
            cfsetospeed(&opt, speed_arr[i]);
        }
        if (i == len)
            UART_Err("Unsupported baud rate.");
    }

    /* data bits */
    switch (data_bits)
    {
    case 8:
        opt.c_cflag |= CS8;
        break;
    case 7:
        opt.c_cflag |= CS7;
        break;
    default:
        UART_Err("Unsupported data bits.");
    }

    /* parity bits */
    switch (parity)
    {
    case 'N':
    case 'n':
        opt.c_iflag &= ~INPCK;
        opt.c_cflag &= ~PARENB;
        break;
    case 'O':
    case 'o':
        opt.c_iflag |= (INPCK|ISTRIP); /*enable parity check, strip parity bits*/
        opt.c_cflag |= (PARODD | PARENB);
        break;
    case 'E':
    case 'e':
        opt.c_iflag |= (INPCK|ISTRIP); /*enable parity check, strip parity bits*/
        opt.c_cflag |= PARENB;
        opt.c_cflag &= ~PARODD;
        break;
    default:
        UART_Err("Unsupported parity bits.");
    }

    /* stop bits */
    switch (stop_bits)
    {
    case 1:
        opt.c_cflag &= ~CSTOPB;
        break;
    case 2:
        opt.c_cflag |= CSTOPB;
        break;
    default:
        UART_Err("Unsupported stop bits.");
    }

    opt.c_lflag =0;// ~(ICANON | ECHO | ECHOE | ISIG);
    opt.c_oflag =0;// ~(OPOST|ONLCR|OCRNL);
    opt.c_iflag =0;
    /*设置等待时间和最小接收字符*/
    opt.c_cc[VMIN] = 0;
    opt.c_cc[VTIME] = 0;

    tcflush(fd, TCIFLUSH);
    ret = tcsetattr(fd, TCSANOW, &opt); /* update it now */
    if (ret < 0)
    {
        UART_Err("Unable to setup the port.");
        return ret;
    }
    UART_Dbg("Setup the port OK!\n");
    return 0; /* everything is OK! */
}

/**
 * @brief serialCom::SerialInit
 * @param device
 * @param speed
 * @param data_bits
 * @param parity
 * @param stop_bits
 * @return 0:success -1:error
 */
int serialCom::SerialInit(const char* device,int speed, int data_bits, char parity, int stop_bits){
    int ret =0;
    gSerialFd=open_port(device);
    if(-1==gSerialFd)
    {
        UART_Err("gSerialFd1 is %d\n",gSerialFd);
        return -1;
    }
//#ifdef NOTEST
    ret = setup_port(gSerialFd, speed, data_bits, parity, stop_bits);  /* setup the port */
    if(!ret){
        UART_Dbg("gSerialFd-setup_port OK\n");
    }else{
        UART_Err("gSerialFd-setup_port failed\n");
    }
//#endif
    return ret;
}

/**
 * @brief serialCom::RecvData
 * @param recvData
 * @return length of received data
 */
int serialCom::RecvData(unsigned char *recvData){
    int recvDataLen;
    recvDataLen = read(gSerialFd, recvData, FramLenMax);
    if(recvDataLen<0||recvDataLen>FramLenMax){
        return -1;
    }
    return recvDataLen;
}

/**
 * @brief serialCom::SendData
 * @param SendData
 * @param rawSendDataLen
 * @return 0:OK -1:ERROR
 */
int serialCom::SendData(unsigned char *SendData,unsigned int SendDataLen){
    int ret=-1;

    ret = write(gSerialFd,SendData,SendDataLen);
    if(ret!=SendDataLen)
    {
        perror("[Error] write data length error %d\n",ret);
        return -1;
    }
    return 0;
}

//serialCom* serialCom::instance_serialCom=NULL;
//serialCom* serialCom::getInstance(){
//    if(instance_serialCom==NULL){
//        instance_serialCom = new serialCom();
//    }
//    return instance_serialCom;
//}
