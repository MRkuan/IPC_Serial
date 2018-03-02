#-------------------------------------------------
#
# Project created by QtCreator 2017-10-23T15:39:48
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = IPC_Serial
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   +=0x11
TEMPLATE = app


SOURCES += \
    src/demo.cpp \
    src/taskManager.cpp \
    src/TLVtools.cpp \
    src/DataLinkLayer.cpp \
    src/TransportLayer.cpp \
    src/COBSTool.cpp \
    src/MsgQueue.cpp \
    src/common.cpp \
    src/serial.cpp \
    src/timeout.cpp \
    src/Msg2MCUhandler.cpp

HEADERS += \
    src/common.h \
    src/taskManager.h \
    src/TLVtools.h \
    src/DataLinkLayer.h \
    src/TransportLayer.h \
    src/COBSTool.h \
    src/MsgQueue.h \
    src/serial.h \
    src/timeout.h \
    src/Msg2MCUhandler.h
