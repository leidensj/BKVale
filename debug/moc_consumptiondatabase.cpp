/****************************************************************************
** Meta object code from reading C++ file 'consumptiondatabase.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../consumptiondatabase.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'consumptiondatabase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConsumptionDatabase_t {
    QByteArrayData data[20];
    char stringdata0[234];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConsumptionDatabase_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConsumptionDatabase_t qt_meta_stringdata_ConsumptionDatabase = {
    {
QT_MOC_LITERAL(0, 0, 19), // "ConsumptionDatabase"
QT_MOC_LITERAL(1, 20, 12), // "filterSignal"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 11), // "chartSignal"
QT_MOC_LITERAL(4, 46, 15), // "QVector<qint64>"
QT_MOC_LITERAL(5, 62, 5), // "vDate"
QT_MOC_LITERAL(6, 68, 15), // "QVector<double>"
QT_MOC_LITERAL(7, 84, 6), // "vTotal"
QT_MOC_LITERAL(8, 91, 14), // "enableControls"
QT_MOC_LITERAL(9, 106, 6), // "insert"
QT_MOC_LITERAL(10, 113, 11), // "Consumption"
QT_MOC_LITERAL(11, 125, 11), // "consumption"
QT_MOC_LITERAL(12, 137, 6), // "remove"
QT_MOC_LITERAL(13, 144, 11), // "updateTotal"
QT_MOC_LITERAL(14, 156, 16), // "emitFilterSignal"
QT_MOC_LITERAL(15, 173, 15), // "emitChartSignal"
QT_MOC_LITERAL(16, 189, 9), // "setFilter"
QT_MOC_LITERAL(17, 199, 19), // "Consumption::Filter"
QT_MOC_LITERAL(18, 219, 6), // "filter"
QT_MOC_LITERAL(19, 226, 7) // "refresh"

    },
    "ConsumptionDatabase\0filterSignal\0\0"
    "chartSignal\0QVector<qint64>\0vDate\0"
    "QVector<double>\0vTotal\0enableControls\0"
    "insert\0Consumption\0consumption\0remove\0"
    "updateTotal\0emitFilterSignal\0"
    "emitChartSignal\0setFilter\0Consumption::Filter\0"
    "filter\0refresh"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConsumptionDatabase[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    2,   65,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   70,    2, 0x08 /* Private */,
       9,    1,   71,    2, 0x08 /* Private */,
      12,    0,   74,    2, 0x08 /* Private */,
      13,    0,   75,    2, 0x08 /* Private */,
      14,    0,   76,    2, 0x08 /* Private */,
      15,    0,   77,    2, 0x08 /* Private */,
      16,    1,   78,    2, 0x0a /* Public */,
      19,    0,   81,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 6,    5,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void,

       0        // eod
};

void ConsumptionDatabase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConsumptionDatabase *_t = static_cast<ConsumptionDatabase *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->filterSignal(); break;
        case 1: _t->chartSignal((*reinterpret_cast< const QVector<qint64>(*)>(_a[1])),(*reinterpret_cast< const QVector<double>(*)>(_a[2]))); break;
        case 2: _t->enableControls(); break;
        case 3: _t->insert((*reinterpret_cast< const Consumption(*)>(_a[1]))); break;
        case 4: _t->remove(); break;
        case 5: _t->updateTotal(); break;
        case 6: _t->emitFilterSignal(); break;
        case 7: _t->emitChartSignal(); break;
        case 8: _t->setFilter((*reinterpret_cast< const Consumption::Filter(*)>(_a[1]))); break;
        case 9: _t->refresh(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<double> >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<qint64> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ConsumptionDatabase::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConsumptionDatabase::filterSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ConsumptionDatabase::*_t)(const QVector<qint64> & , const QVector<double> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConsumptionDatabase::chartSignal)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject ConsumptionDatabase::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_ConsumptionDatabase.data,
      qt_meta_data_ConsumptionDatabase,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ConsumptionDatabase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConsumptionDatabase::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConsumptionDatabase.stringdata0))
        return static_cast<void*>(const_cast< ConsumptionDatabase*>(this));
    return QFrame::qt_metacast(_clname);
}

int ConsumptionDatabase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void ConsumptionDatabase::filterSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ConsumptionDatabase::chartSignal(const QVector<qint64> & _t1, const QVector<double> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
