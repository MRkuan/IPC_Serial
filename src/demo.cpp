#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include "SerialServer.h"
#include "taskManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    taskManager m_taskManager;

    SerialServer* pSerialServer = new SerialServer();

    QDBusConnection connection = QDBusConnection::sessionBus();
    if (!connection.isConnected()){
        qDebug()<<connection.lastError().message();
        return -1;
    }

    //registe service
    if(!connection.registerService("LinuxT.device")){
        qDebug()<<connection.lastError().message();
        return -2;
    }
    //registe object
    if(!connection.registerObject("/serial/service",pSerialServer,
                                   QDBusConnection::ExportAllContents)){
        qDebug()<<connection.lastError().message();
        return -3;
    }
    UART_Dbg("Start D_Bus SerialService successfully\n");

    m_taskManager.start();

    return a.exec();

}
