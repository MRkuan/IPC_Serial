#include "SerialServer.h"

SerialServer::SerialServer()
{

}

SerialServer::~SerialServer()
{

}

/**
 * @brief SerialServer::setDoorInfo
 * @details 设置车门状态
 * @param arg1
 * @param arg2
 * @return
 */
bool SerialServer::setDoorInfo(unsigned short arg1,unsigned short arg2){
    UART_Dbg("called setDoorInfo arg1:%d arg2:%d\n",arg1,arg2);
    mMsg2MCUhandler.sendDoorInfo(arg1,arg2);
    return 1;
}
/**
 * @brief SerialServer::setLampInfo
 * @details 设置车灯状态
 * @param arg1
 * @param arg2
 * @return
 */
bool SerialServer::setLampInfo(unsigned short arg1,unsigned short arg2){
    UART_Dbg("called setLampInfo arg1:%d arg2:%d\n",arg1,arg2);
    mMsg2MCUhandler.sendLampInfo(arg1,arg2);
    return 1;
}
/**
 * @brief SerialServer::setKeyInfo
 * @details 设置车钥匙状态
 * @param arg1
 * @param arg2
 * @return
 */
bool SerialServer::setKeyInfo(unsigned short arg1,unsigned short arg2){
    mMsg2MCUhandler.sendKeyInfo(arg1,arg2);
    return 1;
}
/**
 * @brief SerialServer::setSpeedInfo
 * @details 设置车速状态
 * @param arg1
 * @param arg2
 * @param arg3
 * @return
 */
bool SerialServer::setSpeedInfo(unsigned short arg1,unsigned short arg2,unsigned short arg3){
    mMsg2MCUhandler.sendSpeedInfo(arg1,arg2,arg3);
    return 1;
}
/**
 * @brief SerialServer::setAirConditionerInfo
 * @details 设置空调状态
 * @param arg1
 * @param arg2
 * @param arg3
 * @return
 */
bool SerialServer::setAirConditionerInfo(unsigned short arg1,unsigned short arg2,unsigned short arg3){
    mMsg2MCUhandler.sendAirConditionerInfo(arg1,arg2,arg3);
    return 1;
}
/**
 * @brief SerialServer::setStartInfo
 * @details 设置start状态
 * @param arg1
 * @return
 */
bool SerialServer::setStartInfo(unsigned char arg1){
    mMsg2MCUhandler.sendStartInfo(arg1);
    return 1;
}
