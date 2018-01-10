/***************************************************
* @filename msgqueue.h
* @brief msgqueue
* @author fanjiafei
* @date 2017-11-1
* @version V0.1
****************************************************/

#ifndef MSGQUEUE_H
#define MSGQUEUE_H
#include <queue>

class MsgQueue
{
public:

    int Enqueue(unsigned char* buf,int datalength);
    int Dequeue(unsigned char* buf,int& datalength);
    int Queuelength();
    MsgQueue();
    ~MsgQueue();

private:

    typedef struct _Queue_Station{
        unsigned char * basedata;
        int basedatalength;
    }Queue_Station;
    std::queue < Queue_Station * > Qstation;

};

#endif // MSGQUEUE_H
