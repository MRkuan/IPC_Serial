#ifndef MSG2MCUHANDLER_H
#define MSG2MCUHANDLER_H
#include "common.h"
#include "TransportLayer.h"
class Msg2MCUhandler
{
public:
    Msg2MCUhandler();
    void sendDoorInfo();
    void sendLampInfo();
    void sendKeyInfo();
    void sendSpeedInfo();

private:
    TransportLayer* p_tansportLayer;

};

#endif // MSG2MCUHANDLER_H
