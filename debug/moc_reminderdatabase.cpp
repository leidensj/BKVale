/****************************************************************************
** Meta object code from reading C++ file 'reminderdatabase.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../reminderdatabase.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'reminderdatabase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ReminderTableModel_t {
    QByteArrayData data[1];
    char stringdata0[19];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ReminderTableModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ReminderTableModel_t qt_meta_stringdata_ReminderTableModel = {
    {
QT_MOC_LITERAL(0, 0, 18) // "ReminderTableModel"

    },
    "ReminderTableModel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ReminderTableModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void ReminderTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ReminderTableModel::staticMetaObject = {
    { &QSqlTableModel::staticMetaObject, qt_meta_stringdata_ReminderTableModel.data,
      qt_meta_data_ReminderTableModel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ReminderTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReminderTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ReminderTableModel.stringdata0))
        return static_cast<void*>(const_cast< ReminderTableModel*>(this));
    return QSqlTableModel::qt_metacast(_clname);
}

int ReminderTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSqlTableModel::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_ReminderDatabase_t {
    QByteArrayData data[12];
    char stringdata0[123];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ReminderDatabase_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ReminderDatabase_t qt_meta_stringdata_ReminderDatabase = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ReminderDatabase"
QT_MOC_LITERAL(1, 17, 14), // "selectedSignal"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 8), // "Reminder"
QT_MOC_LITERAL(4, 42, 1), // "r"
QT_MOC_LITERAL(5, 44, 14), // "enableControls"
QT_MOC_LITERAL(6, 59, 18), // "emitSelectedSignal"
QT_MOC_LITERAL(7, 78, 14), // "insertOrUpdate"
QT_MOC_LITERAL(8, 93, 8), // "QString&"
QT_MOC_LITERAL(9, 102, 5), // "error"
QT_MOC_LITERAL(10, 108, 6), // "remove"
QT_MOC_LITERAL(11, 115, 7) // "refresh"

    },
    "ReminderDatabase\0selectedSignal\0\0"
    "Reminder\0r\0enableControls\0emitSelectedSignal\0"
    "insertOrUpdate\0QString&\0error\0remove\0"
    "refresh"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ReminderDatabase[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   47,    2, 0x08 /* Private */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    2,   49,    2, 0x0a /* Public */,
      10,    0,   54,    2, 0x0a /* Public */,
      11,    0,   55,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, 0x80000000 | 3, 0x80000000 | 8,    4,    9,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ReminderDatabase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ReminderDatabase *_t = static_cast<ReminderDatabase *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selectedSignal((*reinterpret_cast< const Reminder(*)>(_a[1]))); break;
        case 1: _t->enableControls(); break;
        case 2: _t->emitSelectedSignal(); break;
        case 3: { bool _r = _t->insertOrUpdate((*reinterpret_cast< const Reminder(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->remove(); break;
        case 5: _t->refresh(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ReminderDatabase::*_t)(const Reminder & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ReminderDatabase::selectedSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ReminderDatabase::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_ReminderDatabase.data,
      qt_meta_data_ReminderDatabase,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ReminderDatabase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReminderDatabase::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ReminderDatabase.stringdata0))
        return static_cast<void*>(const_cast< ReminderDatabase*>(this));
    return QFrame::qt_metacast(_clname);
}

int ReminderDatabase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void ReminderDatabase::selectedSignal(const Reminder & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
