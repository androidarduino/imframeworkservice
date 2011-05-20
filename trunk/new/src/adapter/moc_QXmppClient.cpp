/****************************************************************************
** Meta object code from reading C++ file 'QXmppClient.h'
**
** Created: Fri Dec 31 10:01:56 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../xmpp/src/QXmppClient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppClient[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      25,   12,   12,   12, 0x05,
      40,   12,   12,   12, 0x05,
      82,   66,   12,   12, 0x05,
     117,   12,   12,   12, 0x05,
     147,   12,   12,   12, 0x05,
     179,   12,   12,   12, 0x05,
     199,   12,   12,   12, 0x05,
     246,  237,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     296,   12,  291,   12, 0x0a,
     336,  320,   12,   12, 0x0a,
     368,  365,   12,   12, 0x08,
     408,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QXmppClient[] = {
    "QXmppClient\0\0connected()\0disconnected()\0"
    "error(QXmppClient::Error)\0element,handled\0"
    "elementReceived(QDomElement,bool&)\0"
    "messageReceived(QXmppMessage)\0"
    "presenceReceived(QXmppPresence)\0"
    "iqReceived(QXmppIq)\0"
    "discoveryIqReceived(QXmppDiscoveryIq)\0"
    "type,msg\0logMessage(QXmppLogger::MessageType,QString)\0"
    "bool\0sendPacket(QXmppPacket)\0"
    "bareJid,message\0sendMessage(QString,QString)\0"
    "iq\0invokeInterfaceMethod(QXmppRpcInvokeIq)\0"
    "xmppConnected()\0"
};

const QMetaObject QXmppClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QXmppClient,
      qt_meta_data_QXmppClient, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppClient))
        return static_cast<void*>(const_cast< QXmppClient*>(this));
    return QObject::qt_metacast(_clname);
}

int QXmppClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: connected(); break;
        case 1: disconnected(); break;
        case 2: error((*reinterpret_cast< QXmppClient::Error(*)>(_a[1]))); break;
        case 3: elementReceived((*reinterpret_cast< const QDomElement(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: messageReceived((*reinterpret_cast< const QXmppMessage(*)>(_a[1]))); break;
        case 5: presenceReceived((*reinterpret_cast< const QXmppPresence(*)>(_a[1]))); break;
        case 6: iqReceived((*reinterpret_cast< const QXmppIq(*)>(_a[1]))); break;
        case 7: discoveryIqReceived((*reinterpret_cast< const QXmppDiscoveryIq(*)>(_a[1]))); break;
        case 8: logMessage((*reinterpret_cast< QXmppLogger::MessageType(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: { bool _r = sendPacket((*reinterpret_cast< const QXmppPacket(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: sendMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 11: invokeInterfaceMethod((*reinterpret_cast< const QXmppRpcInvokeIq(*)>(_a[1]))); break;
        case 12: xmppConnected(); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void QXmppClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QXmppClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QXmppClient::error(QXmppClient::Error _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QXmppClient::elementReceived(const QDomElement & _t1, bool & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QXmppClient::messageReceived(const QXmppMessage & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QXmppClient::presenceReceived(const QXmppPresence & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QXmppClient::iqReceived(const QXmppIq & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QXmppClient::discoveryIqReceived(const QXmppDiscoveryIq & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void QXmppClient::logMessage(QXmppLogger::MessageType _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
