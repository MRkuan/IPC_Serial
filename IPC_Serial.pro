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
    demo.cpp \
    taskManager.cpp \
    TLVtools.cpp \
    DataLinkLayer.cpp \
    TransportLayer.cpp \
    COBSTool.cpp \
    MsgQueue.cpp \
    SerialCom.cpp \
    common.cpp

HEADERS += \
    common.h \
    taskManager.h \
    TLVtools.h \
    DataLinkLayer.h \
    TransportLayer.h \
    COBSTool.h \
    MsgQueue.h \
    SerialCom.h
