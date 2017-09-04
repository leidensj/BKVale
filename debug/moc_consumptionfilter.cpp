/****************************************************************************
** Meta object code from reading C++ file 'consumptionfilter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../consumptionfilter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'consumptionfilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConsumptionFilter_t {
    QByteArrayData data[12];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConsumptionFilter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConsumptionFilter_t qt_meta_stringdata_ConsumptionFilter = {
    {
QT_MOC_LITERAL(0, 0, 17), // "ConsumptionFilter"
QT_MOC_LITERAL(1, 18, 13), // "changedSignal"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 7), // "bEnable"
QT_MOC_LITERAL(4, 41, 5), // "datei"
QT_MOC_LITERAL(5, 47, 5), // "datef"
QT_MOC_LITERAL(6, 53, 11), // "chartSignal"
QT_MOC_LITERAL(7, 65, 17), // "emitChangedSignal"
QT_MOC_LITERAL(8, 83, 15), // "emitChartSignal"
QT_MOC_LITERAL(9, 99, 14), // "enableControls"
QT_MOC_LITERAL(10, 114, 11), // "updateTotal"
QT_MOC_LITERAL(11, 126, 5) // "total"

    },
    "ConsumptionFilter\0changedSignal\0\0"
    "bEnable\0datei\0datef\0chartSignal\0"
    "emitChangedSignal\0emitChartSignal\0"
    "enableControls\0updateTotal\0total"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConsumptionFilter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   44,    2, 0x06 /* Public */,
       6,    3,   51,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   58,    2, 0x08 /* Private */,
       8,    0,   59,    2, 0x08 /* Private */,
       9,    0,   60,    2, 0x0a /* Public */,
      10,    1,   61,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::LongLong, QMetaType::LongLong,    3,    4,    5,
    QMetaType::Void, QMetaType::Bool, QMetaType::LongLong, QMetaType::LongLong,    3,    4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   11,

       0        // eod
};

void ConsumptionFilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConsumptionFilter *_t = static_cast<ConsumptionFilter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changedSignal((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2])),(*reinterpret_cast< qint64(*)>(_a[3]))); break;
        case 1: _t->chartSignal((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2])),(*reinterpret_cast< qint64(*)>(_a[3]))); break;
        case 2: _t->emitChangedSignal(); break;
        case 3: _t->emitChartSignal(); break;
        case 4: _t->enableControls(); break;
        case 5: _t->updateTotal((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ConsumptionFilter::*_t)(bool , qint64 , qint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConsumptionFilter::changedSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ConsumptionFilter::*_t)(bool , qint64 , qint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConsumptionFilter::chartSignal)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject ConsumptionFilter::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_ConsumptionFilter.data,
      qt_meta_data_ConsumptionFilter,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ConsumptionFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConsumptionFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConsumptionFilter.stringdata0))
        return static_cast<void*>(const_cast< ConsumptionFilter*>(this));
    return QFrame::qt_metacast(_clname);
}

int ConsumptionFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ConsumptionFilter::changedSignal(bool _t1, qint64 _t2, qint64 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConsumptionFilter::chartSignal(bool _t1, qint64 _t2, qint64 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
