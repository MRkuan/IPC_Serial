#include "SerialServer.h"

SerialServer::SerialServer()
{

}

SerialServer::~SerialServer()
{

}


bool SerialServer::setDoorInfo(unsigned short arg1,unsigned short arg2){
    UART_Dbg("called setDoorInfo arg1:%d arg2:%d\n",arg1,arg2);
    mMsg2MCUhandler.sendDoorInfo(arg1,arg2);
    return 1;
}

bool SerialServer::setLampInfo(unsigned short arg1,unsigned short arg2){
    UART_Dbg("called setLampInfo arg1:%d arg2:%d\n",arg1,arg2);
    mMsg2MCUhandler.sendLampInfo(arg1,arg2);
    return 1;
}

bool SerialServer::setKeyInfo(unsigned short arg1,unsigned short arg2){
    mMsg2MCUhandler.sendKeyInfo(arg1,arg2);
    return 1;
}

bool SerialServer::setSpeedInfo(unsigned short arg1,unsigned short arg2,unsigned short arg3){
    mMsg2MCUhandler.sendSpeedInfo(arg1,arg2,arg3);
    return 1;
}

bool SerialServer::setAirConditionerInfo(unsigned short arg1,unsigned short arg2,unsigned short arg3){
    mMsg2MCUhandler.sendAirConditionerInfo(arg1,arg2,arg3);
    return 1;
}

bool SerialServer::setStartInfo(unsigned char arg1){
    mMsg2MCUhandler.sendStartInfo(arg1);
    return 1;
}
