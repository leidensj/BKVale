/****************************************************************************
** Meta object code from reading C++ file 'historywidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../historywidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'historywidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HistoryTableModel_t {
    QByteArrayData data[1];
    char stringdata0[18];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HistoryTableModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HistoryTableModel_t qt_meta_stringdata_HistoryTableModel = {
    {
QT_MOC_LITERAL(0, 0, 17) // "HistoryTableModel"

    },
    "HistoryTableModel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HistoryTableModel[] = {

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

void HistoryTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject HistoryTableModel::staticMetaObject = {
    { &QSqlTableModel::staticMetaObject, qt_meta_stringdata_HistoryTableModel.data,
      qt_meta_data_HistoryTableModel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HistoryTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HistoryTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HistoryTableModel.stringdata0))
        return static_cast<void*>(const_cast< HistoryTableModel*>(this));
    return QSqlTableModel::qt_metacast(_clname);
}

int HistoryTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSqlTableModel::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_HistoryWidget_t {
    QByteArrayData data[8];
    char stringdata0[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HistoryWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HistoryWidget_t qt_meta_stringdata_HistoryWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "HistoryWidget"
QT_MOC_LITERAL(1, 14, 18), // "noteSelectedSignal"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 4), // "Note"
QT_MOC_LITERAL(4, 39, 4), // "note"
QT_MOC_LITERAL(5, 44, 7), // "refresh"
QT_MOC_LITERAL(6, 52, 12), // "noteSelected"
QT_MOC_LITERAL(7, 65, 3) // "idx"

    },
    "HistoryWidget\0noteSelectedSignal\0\0"
    "Note\0note\0refresh\0noteSelected\0idx"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HistoryWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   37,    2, 0x0a /* Public */,
       6,    1,   38,    2, 0x08 /* Private */,
       6,    0,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    7,
    QMetaType::Void,

       0        // eod
};

void HistoryWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HistoryWidget *_t = static_cast<HistoryWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->noteSelectedSignal((*reinterpret_cast< const Note(*)>(_a[1]))); break;
        case 1: _t->refresh(); break;
        case 2: _t->noteSelected((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->noteSelected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (HistoryWidget::*_t)(const Note & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HistoryWidget::noteSelectedSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject HistoryWidget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_HistoryWidget.data,
      qt_meta_data_HistoryWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HistoryWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HistoryWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HistoryWidget.stringdata0))
        return static_cast<void*>(const_cast< HistoryWidget*>(this));
    return QFrame::qt_metacast(_clname);
}

int HistoryWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void HistoryWidget::noteSelectedSignal(const Note & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
