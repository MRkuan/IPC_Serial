/**

* @file       Msg2MCUhandler.h

* @brief      处理发送给MCU的消息

* @details    封装外部发送给MCU的消息，提供接口供发送函数调用

* @author     Albert

*/
#ifndef MSG2MCUHANDLER_H
#define MSG2MCUHANDLER_H
#include "common.h"
#include "TransportLayer.h"
class Msg2MCUhandler
{
public:
    Msg2MCUhandler();
    void sendDoorInfo(unsigned short arg1,unsigned short arg2);
    void sendLampInfo(unsigned short arg1,unsigned short arg2);
    void sendKeyInfo(unsigned short arg1,unsigned short arg2);
    void sendSpeedInfo(unsigned short arg1,unsigned short arg2,unsigned short arg3);
    void sendAirConditionerInfo(unsigned short arg1,unsigned short arg2,unsigned short arg3);
    void sendStartInfo(unsigned char arg1);

private:
    TransportLayer* p_tansportLayer;

};

#endif // MSG2MCUHANDLER_H
