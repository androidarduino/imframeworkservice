/****************************************************************************
** Meta object code from reading C++ file 'irc.h'
**
** Created: Mon Jan 10 10:14:59 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../irc/irc.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'irc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_irc__IRCClient[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      42,   16,   15,   15, 0x05,
      87,   83,   15,   15, 0x05,
     124,  109,   15,   15, 0x05,
     152,   15,   15,   15, 0x05,
     169,  164,   15,   15, 0x05,
     205,  184,   15,   15, 0x05,
     235,   15,   15,   15, 0x05,
     250,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     260,   15,   15,   15, 0x08,
     273,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_irc__IRCClient[] = {
    "irc::IRCClient\0\0from,fromURI,receiver,msg\0"
    "message(QString,QString,QString,QString)\0"
    "msg\0notification(QString)\0from,toChannel\0"
    "invitation(QString,QString)\0connected()\0"
    "ping\0ping(QString&)\0username,channel,uri\0"
    "join(QString,QString,QString)\0"
    "disconnected()\0updated()\0msgArrived()\0"
    "login()\0"
};

const QMetaObject irc::IRCClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_irc__IRCClient,
      qt_meta_data_irc__IRCClient, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &irc::IRCClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *irc::IRCClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *irc::IRCClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_irc__IRCClient))
        return static_cast<void*>(const_cast< IRCClient*>(this));
    return QObject::qt_metacast(_clname);
}

int irc::IRCClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: message((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 1: notification((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: invitation((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: connected(); break;
        case 4: ping((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: join((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 6: disconnected(); break;
        case 7: updated(); break;
        case 8: msgArrived(); break;
        case 9: login(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void irc::IRCClient::message(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void irc::IRCClient::notification(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void irc::IRCClient::invitation(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void irc::IRCClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void irc::IRCClient::ping(QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void irc::IRCClient::join(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void irc::IRCClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void irc::IRCClient::updated()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}
QT_END_MOC_NAMESPACE
