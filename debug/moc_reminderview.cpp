/****************************************************************************
** Meta object code from reading C++ file 'reminderview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../reminderview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'reminderview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ReminderView_t {
    QByteArrayData data[12];
    char stringdata0[143];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ReminderView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ReminderView_t qt_meta_stringdata_ReminderView = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ReminderView"
QT_MOC_LITERAL(1, 13, 13), // "changedSignal"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 19), // "searchClickedSignal"
QT_MOC_LITERAL(4, 48, 17), // "emitChangedSignal"
QT_MOC_LITERAL(5, 66, 23), // "emitSearchClickedSignal"
QT_MOC_LITERAL(6, 90, 17), // "setCapitalization"
QT_MOC_LITERAL(7, 108, 5), // "state"
QT_MOC_LITERAL(8, 114, 11), // "setReminder"
QT_MOC_LITERAL(9, 126, 8), // "Reminder"
QT_MOC_LITERAL(10, 135, 1), // "r"
QT_MOC_LITERAL(11, 137, 5) // "clear"

    },
    "ReminderView\0changedSignal\0\0"
    "searchClickedSignal\0emitChangedSignal\0"
    "emitSearchClickedSignal\0setCapitalization\0"
    "state\0setReminder\0Reminder\0r\0clear"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ReminderView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    1,   53,    2, 0x08 /* Private */,
       8,    1,   56,    2, 0x0a /* Public */,
      11,    0,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Int,
    QMetaType::Int,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,

       0        // eod
};

void ReminderView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ReminderView *_t = static_cast<ReminderView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->changedSignal();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 1: { int _r = _t->searchClickedSignal();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->emitChangedSignal(); break;
        case 3: _t->emitSearchClickedSignal(); break;
        case 4: _t->setCapitalization((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setReminder((*reinterpret_cast< const Reminder(*)>(_a[1]))); break;
        case 6: _t->clear(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef int (ReminderView::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ReminderView::changedSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef int (ReminderView::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ReminderView::searchClickedSignal)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject ReminderView::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_ReminderView.data,
      qt_meta_data_ReminderView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ReminderView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReminderView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ReminderView.stringdata0))
        return static_cast<void*>(const_cast< ReminderView*>(this));
    return QFrame::qt_metacast(_clname);
}

int ReminderView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
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
int ReminderView::changedSignal()
{
    int _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
    return _t0;
}

// SIGNAL 1
int ReminderView::searchClickedSignal()
{
    int _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
    return _t0;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
