/****************************************************************************
** Meta object code from reading C++ file 'jabberplugin.h'
**
** Created: Thu Jan 10 21:51:26 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "jabberplugin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jabberplugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_IMJabberPlugin[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   16,   15,   15, 0x05,
      44,   37,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      93,   67,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_IMJabberPlugin[] = {
    "IMJabberPlugin\0\0msg\0msgArrived(Msg&)\0"
    "status\0statusChanged(QString)\0"
    "from,fromUri,receiver,msg\0"
    "gotMsg(QString,QString,QString,QString)\0"
};

const QMetaObject IMJabberPlugin::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IMJabberPlugin,
      qt_meta_data_IMJabberPlugin, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &IMJabberPlugin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *IMJabberPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *IMJabberPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IMJabberPlugin))
        return static_cast<void*>(const_cast< IMJabberPlugin*>(this));
    if (!strcmp(_clname, "IMProtocol"))
        return static_cast< IMProtocol*>(const_cast< IMJabberPlugin*>(this));
    if (!strcmp(_clname, "com.vrcats.IMFramework.IMProtocol/1.0"))
        return static_cast< IMProtocol*>(const_cast< IMJabberPlugin*>(this));
    return QObject::qt_metacast(_clname);
}

int IMJabberPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: msgArrived((*reinterpret_cast< Msg(*)>(_a[1]))); break;
        case 1: statusChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: gotMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void IMJabberPlugin::msgArrived(Msg & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IMJabberPlugin::statusChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
