#ifndef SERIALSERVER_H
#define SERIALSERVER_H
#include <QObject>
#include <QString>

#include "common.h"
#include "Msg2MCUhandler.h"



class SerialServer: public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "Serial.interface")
public:
    SerialServer();
    ~SerialServer();

public slots:
    bool setDoorInfo(unsigned short arg1,unsigned short arg2);
    bool setLampInfo(unsigned short arg1,unsigned short arg2);
    bool setKeyInfo(unsigned short arg1,unsigned short arg2);
    bool setSpeedInfo(unsigned short arg1,unsigned short arg2,unsigned short arg3);
    bool setAirConditionerInfo(unsigned short arg1,unsigned short arg2,unsigned short arg3);
    bool setStartInfo(unsigned char arg1);
signals:
    /**
     * @brief broadcastCANInfo
     * @details 广播CAN数据
     * @param candata
     * @param len
     */
    void broadcastCANInfo(QByteArray candata,unsigned char len);

private:
    Msg2MCUhandler mMsg2MCUhandler;

};

#endif // SERIALSERVER_H
