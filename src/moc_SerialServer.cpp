/****************************************************************************
** Meta object code from reading C++ file 'SerialServer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "SerialServer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SerialServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SerialServer_t {
    QByteArrayData data[16];
    char stringdata0[174];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialServer_t qt_meta_stringdata_SerialServer = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SerialServer"
QT_MOC_LITERAL(1, 13, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 29, 16), // "Serial.interface"
QT_MOC_LITERAL(3, 46, 16), // "broadcastCANInfo"
QT_MOC_LITERAL(4, 63, 0), // ""
QT_MOC_LITERAL(5, 64, 7), // "candata"
QT_MOC_LITERAL(6, 72, 3), // "len"
QT_MOC_LITERAL(7, 76, 11), // "setDoorInfo"
QT_MOC_LITERAL(8, 88, 4), // "arg1"
QT_MOC_LITERAL(9, 93, 4), // "arg2"
QT_MOC_LITERAL(10, 98, 11), // "setLampInfo"
QT_MOC_LITERAL(11, 110, 10), // "setKeyInfo"
QT_MOC_LITERAL(12, 121, 12), // "setSpeedInfo"
QT_MOC_LITERAL(13, 134, 4), // "arg3"
QT_MOC_LITERAL(14, 139, 21), // "setAirConditionerInfo"
QT_MOC_LITERAL(15, 161, 12) // "setStartInfo"

    },
    "SerialServer\0D-Bus Interface\0"
    "Serial.interface\0broadcastCANInfo\0\0"
    "candata\0len\0setDoorInfo\0arg1\0arg2\0"
    "setLampInfo\0setKeyInfo\0setSpeedInfo\0"
    "arg3\0setAirConditionerInfo\0setStartInfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialServer[] = {

 // content:
       7,       // revision
       0,       // classname
       1,   14, // classinfo
       7,   16, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags
       3,    2,   51,    4, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    2,   56,    4, 0x0a /* Public */,
      10,    2,   61,    4, 0x0a /* Public */,
      11,    2,   66,    4, 0x0a /* Public */,
      12,    3,   71,    4, 0x0a /* Public */,
      14,    3,   78,    4, 0x0a /* Public */,
      15,    1,   85,    4, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray, QMetaType::UChar,    5,    6,

 // slots: parameters
    QMetaType::Bool, QMetaType::UShort, QMetaType::UShort,    8,    9,
    QMetaType::Bool, QMetaType::UShort, QMetaType::UShort,    8,    9,
    QMetaType::Bool, QMetaType::UShort, QMetaType::UShort,    8,    9,
    QMetaType::Bool, QMetaType::UShort, QMetaType::UShort, QMetaType::UShort,    8,    9,   13,
    QMetaType::Bool, QMetaType::UShort, QMetaType::UShort, QMetaType::UShort,    8,    9,   13,
    QMetaType::Bool, QMetaType::UChar,    8,

       0        // eod
};

void SerialServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SerialServer *_t = static_cast<SerialServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->broadcastCANInfo((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< unsigned char(*)>(_a[2]))); break;
        case 1: { bool _r = _t->setDoorInfo((*reinterpret_cast< unsigned short(*)>(_a[1])),(*reinterpret_cast< unsigned short(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 2: { bool _r = _t->setLampInfo((*reinterpret_cast< unsigned short(*)>(_a[1])),(*reinterpret_cast< unsigned short(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 3: { bool _r = _t->setKeyInfo((*reinterpret_cast< unsigned short(*)>(_a[1])),(*reinterpret_cast< unsigned short(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->setSpeedInfo((*reinterpret_cast< unsigned short(*)>(_a[1])),(*reinterpret_cast< unsigned short(*)>(_a[2])),(*reinterpret_cast< unsigned short(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->setAirConditionerInfo((*reinterpret_cast< unsigned short(*)>(_a[1])),(*reinterpret_cast< unsigned short(*)>(_a[2])),(*reinterpret_cast< unsigned short(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->setStartInfo((*reinterpret_cast< unsigned char(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SerialServer::*_t)(QByteArray , unsigned char );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialServer::broadcastCANInfo)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject SerialServer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SerialServer.data,
      qt_meta_data_SerialServer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SerialServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialServer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SerialServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void SerialServer::broadcastCANInfo(QByteArray _t1, unsigned char _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
